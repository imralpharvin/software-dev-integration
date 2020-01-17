'use strict'

// C library API
const ffi = require('ffi');

//File Log table
var fileList = [];

// Express App (Routes)
const express = require("express");
const app = express();
const path = require("path");
const fileUpload = require('express-fileupload');
const bodyParser = require('body-parser');

app.use(fileUpload());
app.use(bodyParser.json());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/', function (req, res) {
  res.sendFile(path.join(__dirname + '/public/index.html'));
});

// Send Style, do not change
app.get('/style.css', function (req, res) {
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname + '/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js', function (req, res) {
  fs.readFile(path.join(__dirname + '/public/index.js'), 'utf8', function (err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, { compact: true, controlFlowFlattening: true });
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function (req, res) {
  //console.log(req.files);
  if (!req.files) {
    return res.status(400).send('No files were uploaded.');
  }

  let uploadFile = req.files.uploadFile;

  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/' + uploadFile.name, function (err) {
    if (err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function (req, res) {
  fs.stat('uploads/' + req.params.name, function (err, stat) {
    console.log(err);
    if (err == null) {
      res.sendFile(path.join(__dirname + '/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ********************

//Sample endpoint
app.get('/someendpoint', function (req, res) {
  res.send({
    foo: "bar"
  });
});


let calParser = ffi.Library('./libcal', {
  'ffiTest': ['string', []],
  'filenameToJSON': ['string', ['string']],
  'filenameToEventListJSON': ['string', ['string']],
  'createAndWriteCalendar': ['void', ['string', 'string', 'string', 'string', 'string']],
  'addEventAndWriteCalendar': ['void', ['string', 'string', 'string', 'string']],
});


function parseFLTable(returnList) {
  var fileList = [];
  const testFolder = './uploads/';
  const pathFile = "../uploads/";
  const appjsFile = "uploads/";

  let calendar;

  //console.log("parseFLTable works");
  fs.readdir(testFolder, (err, files) => {
    files.forEach(file => {
      //console.log(pathFile + file);
      let string = appjsFile + file;
      let c = calParser.filenameToJSON(string);
      //console.log(c);
      let calendar = JSON.parse(c);

      //console.log("READDIR WORKS");

      if (calendar["version"] == undefined) {
        calendar["prodID"] = "invalid file";
        calendar["version"] = "";
        calendar["numEvents"] = "";
        calendar["numProps"] = "";
      }
      calendar["filename"] = file;

      //console.log(calendar);
      fileList.push(calendar);
      //console.log("FILELIST: " + fileList);

      //console.log("hey" + fileList[0]);
      //console.log(fileList.length);
    });


    return returnList(fileList);
  });

}



app.get('/calendarData', function (req, res) {

  parseFLTable(function (fileList) {
    //console.log(fileList); // this is where you get the return value
    res.send(fileList);
  });

  /*console.log("CALENDAR DATA: " + calendarData);

  res.send(calendarData);*/
});


app.get('/updateData', function (req, res) {
  var fileList = [];
  //console.log(fileList);
  res.send(fileList);
});

var string = {};

app.post('/eventFilename', function (req, res) {
  string = req.body;
  console.log("POST:" + string["filename"]);



  //var testParsed = geteventListData("uploads/" +  string["filename"])
  //console.log(testParsed);
  res.send(req.body);
});

app.get('/eventListData', function (req, res) {
  console.log("GET:" + string["filename"]);
  //console.log("uploads/" + string);
  if (string["filename"] == "") {
    var emptyEventList = [];
    console.log("GET EVENT LIST" + string["filename"]);
    res.send(emptyEventList);
  }
  else {
    var testParsed = geteventListData("uploads/" + string["filename"]);
    console.log("GET EVENT LIST:MORE THAN ONE " + testParsed);
    res.send(testParsed);
  }

});

function geteventListData(filename) {
  var test = calParser.filenameToEventListJSON(filename);

  console.log("TEST: " + test);
  var testParsed = JSON.parse(test);
  //console.log("TEST parsed: " +testParsed);
  return testParsed;
}

var string = "{\"version\":2,\"prodID\":\"-//Mozilla.org/NONSGML Mozilla Calendar V1.1//EN\", \"eventList\":[{\"UID\":\"1234\",\"startDT\":{\"date\":\"20151002\",\"time\":\"100000\",\"isUTC\":false},\"createDT\":{\"date\":\"20151002\",\"time\":\"100000\",\"isUTC\":true}}]}";

var parsed = JSON.parse(string);
//console.log(parsed);
/*
function createCalendar(filename, json) {
  var calendar = {};
  calendar["version"] = parsed["version"];
  calendar["prodID"] = parsed["prodID"];
  console.log(JSON.stringify(calendar));

  var eventUID = {};
  eventUID["UID"] = parsed["eventList"][0]["UID"];
  console.log(JSON.stringify(eventUID));

  var dtStart = {};
  dtStart["date"] = parsed["eventList"][0]["startDT"]["date"];
  dtStart["time"] = parsed["eventList"][0]["startDT"]["time"];
  dtStart["isUTC"] = parsed["eventList"][0]["startDT"]["isUTC"];

  var dtStamp = {};
  dtStamp["date"] = parsed["eventList"][0]["createDT"]["date"];
  dtStamp["time"] = parsed["eventList"][0]["createDT"]["time"];
  dtStamp["isUTC"] = parsed["eventList"][0]["createDT"]["isUTC"];

  console.log(JSON.stringify(dtStart));
  console.log(JSON.stringify(dtStamp));



  calParser.createAndWriteCalendar("uploads/appjs.ics", JSON.stringify(calendar), JSON.stringify(eventUID), JSON.stringify(dtStart), JSON.stringify(dtStamp));

}

createCalendar("naaa", "wasted");
*/

app.post('/createCalendarInfo', function (req, res) {
  var string = req.body;
  //console.log("POSTccI:" + string["prodID"] + string["UID"]);
  //console.log(JSON.stringify(string));
  var filenameJSON = {};
  filenameJSON["filename"] = "wokkkks.ics"
  res.send(filenameJSON);


  var filename = string["filename"];

  //console.log(filename);
  var calendarInfo = {};
  calendarInfo["version"] = Number(string["version"]);
  calendarInfo["prodID"] = string["prodID"];
  console.log(JSON.stringify(calendarInfo));

  var eventInfo = {};
  eventInfo["UID"] = string["UID"];
  console.log(JSON.stringify(eventInfo));
  var dtStartInfo = {};
  dtStartInfo["date"] = string["startDate"];
  dtStartInfo["time"] = string["startTime"];
  dtStartInfo["isUTC"] = string["startUTC"];
  console.log(JSON.stringify(dtStartInfo));
  var dtStampInfo = {};
  dtStampInfo["date"] = string["createDate"];
  dtStampInfo["time"] = string["createTime"];
  dtStampInfo["isUTC"] = string["createUTC"];
  console.log(JSON.stringify(dtStampInfo));

  calParser.createAndWriteCalendar("uploads/" + filename, JSON.stringify(calendarInfo), JSON.stringify(eventInfo), JSON.stringify(dtStartInfo), JSON.stringify(dtStampInfo));

});

app.post('/addEventInfo', function (req, res) {
  var string = req.body;
  //console.log("POSTccI:" + string["prodID"] + string["UID"]);
  //console.log(JSON.stringify(string));
  var filenameJSON = {};
  filenameJSON["filename"] = "wokkkks.ics"
  res.send(filenameJSON);


  var filename = string["filename"];

  //console.log(filename);

  var eventInfo = {};
  eventInfo["UID"] = string["UID"];
  console.log(JSON.stringify(eventInfo));
  var dtStartInfo = {};
  dtStartInfo["date"] = string["startDate"];
  dtStartInfo["time"] = string["startTime"];
  dtStartInfo["isUTC"] = string["startUTC"];
  console.log(JSON.stringify(dtStartInfo));
  var dtStampInfo = {};
  dtStampInfo["date"] = string["createDate"];
  dtStampInfo["time"] = string["createTime"];
  dtStampInfo["isUTC"] = string["createUTC"];
  console.log(JSON.stringify(dtStampInfo));

  calParser.addEventAndWriteCalendar("uploads/" + filename, JSON.stringify(eventInfo), JSON.stringify(dtStartInfo), JSON.stringify(dtStampInfo));

});



//createCalendar("uploads/yap.ics", "doesnt matter");



let c = "foo";

app.get('/ffiTest', function (req, res) {
  res.send({
    foo: c
  });
});
app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
