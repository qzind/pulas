
const MSG_UNKNOWN = 0, MSG_LIST_PRINT = 1, MSG_SELECT_PRINT = 2, MSG_GET_DEFAULT_PRINT = 3,
	MSG_GET_CURRENT_PRINT = 4, MSG_SETTING_PRINT = 5, MSG_PRINT = 6;

window.Pulas = function() {
	this.port = 1963;
	this.host = 'localhost';
	this.available = false;
	this._callback = {};
	this._queue = [];
	this._processing = false;
	this._connectionCallback = null;
}

Pulas.prototype = {
	isAvailable: function() {
		return this.available;
	},
	connect: function(callback) {
		this.ws = new WebSocket('ws://' + this.host + ':' + this.port);
		this.ws.onopen = this._onopen.bind(this);
		this.ws.onclose = this._onclose.bind(this);
		this.ws.onmessage = this._onmessage.bind(this);
		if(typeof callback != 'undefined')
			this._connectionCallback = callback;
	},
	_onopen: function() {
		this.available = true;
		this._shiftQueue();
		if(this._connectionCallback != null && typeof this._connectionCallback == 'function')
			this._connectionCallback('opened');
	},
	_onclose: function() {
		this.available = false;
		if(this._connectionCallback != null && typeof this._connectionCallback == 'function')
			this._connectionCallback('closed');
	},
	_onmessage: function(msg) {
		var d = JSON.parse(msg.data);
		if(d.token in this._callback) {
			if(typeof this._callback[d.token] == 'function') {
				this._callback[d.token](d);
			}
			delete this._callback[d.token];
		}
		this._processing = false;
		this._shiftQueue();
	},
	_shiftQueue: function() {
		if(this._queue.length > 0) {
			var obj = this._queue.shift();
			this._send(obj.data, obj.callback);
		}
	},
	_send: function(data, callback) {
		if(this.ws.readyState == 0 || this._processing) {
			this._queue.push({data: data, callback: callback});
			return;
		} else if(this.ws.readyState == 2 || this.ws.readyState == 3) {
			return;
		}
		var token = (new Date()).getTime();
		data.token = token;
		this._processing = true;
		this.ws.send(JSON.stringify(data));
		if(typeof callback == 'function') {
			this._callback[token] = callback;
		}
	},
	getPrinterList: function(callback) {
		this._send({type: MSG_LIST_PRINT}, callback);
	},
	selectPrint: function(printName, callback) {
		this._send({type: MSG_SELECT_PRINT, print: printName}, callback);
	},
	getDefaultPrinter: function(callback) {
		this._send({type: MSG_GET_DEFAULT_PRINT}, callback);
	},
	getCurrentPrinter: function(callback) {
		this._send({type: MSG_GET_CURRENT_PRINT}, callback);
	},
	/*
	* available setting object :
	* papersize: A4, A5, Letter
	* margin: {top, bottom, left, right}
	* unit: mm, inch
	* orientation: portrait, landscape
	*/
	settingPrinter: function(setting, callback) {
		this._send({type: MSG_SETTING_PRINT, setting: setting}, callback);
	},
	printHtml: function(html, callback) {
		this._send({type: MSG_PRINT, data: {data: html}}, callback);
	},
	printHtmlToPdf: function(html, callback) {
		this._send({type: MSG_PRINT, data: {data: html, printoutput: "pdf"}}, callback);
	},
	printHtmlToPdfAndDownload: function(html) {
		var that = this;
		this._send({type: MSG_PRINT, data: {data: html, printoutput: "pdf"}},
			function(data) { that._downloadPdf(data.data); });
	},
	printRaw: function(data, callback) {
		this._send({type: MSG_PRINT, data: {data: data, printtype: "escp"}}, callback);
	},
	_downloadPdf: function(data) {
		window.open('data:application/pdf;base64,' + data, '_blank');
	}
}