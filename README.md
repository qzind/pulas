# pulas

Pulas is a Qt application and Javascript library that allow to print directly from Javascript (on your browser) to the printer without open any printing dialog. The application is using WebKit for rendering the content of html. So, it can contain live html means that it will able to render image that has link to live website. And it able to print any html or html5 to pdf.

### Features
  -  fully html and css compatible (also the js inside it)
  -  select and configure printer from the javascript
  -  print to pdf and download to the browser
  -  print raw (example sending ESC/P command to dot matrix)

### Binary Download
  -  Please go to [pulas.lekapin.com](http://pulas.lekapin.com)

### Get started
  -  Download the binary package
  -  Run the pulasQt/pulas.exe and make sure to allow access network.
  -  Open pulasJS/example/full.html (make sure you have internet connection)
    
### Javascript API
Initialize pulas
``` javascript
var pulas = new Pulas();
```
Connect to pulas server
``` javascript
pulas.connect(function(value) {
    //value = 'openned' -> if successfully open
    //value = 'closed' -> if can not connect to server
});
```
Get list of printer
``` javascript
pulas.getPrinterList(function(value) {
    //example output of value :
    //value.data = ["Epson", "Canon"]
});
```
Get default printer
``` javascript
pulas.getDefaultPrinter(function(value) {
    //example output of value :
    //value.data = "Epson LX-310"
});
```
Select printer to use for printing
``` javascript
pulas.selectPrint("Canon Pixma", function(value) {
});
```
Get current selected printer
``` javascript
pulas.getCurrentPrinter(function(value) {
    //example output of value :
    //value.data = "Epson LX-310"
});
```
Setting printer
``` javascript
var setting = {
	papersize: 'A4',
	unit: 'mm',
	margin: {
		top: 10,
		bottom: 10,
		left: 10,
		right: 10
	},
	orientation: 'landscape'
};
pulas.settingPrinter(setting, function(value) {
});
```
Print html to printer
``` javascript
pulas.printHtml(html, function(value) {
});
```
Print html to pdf
``` javascript
pulas.printHtmlToPdfAndDownload(html, function(value) {
});
```
Print raw command to printer
``` javascript
var rawcommand = '\x1b@This is simple ESCP command';
pulas.printHtmlToPdfAndDownload(rawcommand, function(value) {
});
```

### Printer setting
unit
```
unit : "mm", "inch"
```
papersize
```
papersize : "A4", "A5", "Letter"
```
custom papersize
```
papersize : { width: 200, height: 200 }
```
margin
```
margin : { top: 10, right: 10, bottom: 10, left: 10 }
```
orientation
```
orientation: "landscape", "portrait"
```

### Todos
  -  [x] build binary that ready to use by user
  -  [x] documentation how to use it
  -  [x] example html js
  -  [x] build javascript using gulp for optimizing
  -  [ ] print from pdf
  -  [ ] any suggestion?

### License
  -  LGPL

### Author
  -  apin (apin.klas@gmail.com)

tags: html5 to pdf, javascript printing
