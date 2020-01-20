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
const mysql = require('mysql');


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

function parseFLTableSync(){
  var fileList = [];
  const testFolder = './uploads/';

  let calendar;
  let files = fs.readdirSync(testFolder);

  if(files == undefined|| files == null)
  {
    return null;
  }

  files.forEach(file => {
      let string = "uploads/" + file;
      let c = calParser.filenameToJSON(string);
      let calendar = JSON.parse(c);

      if (calendar["version"] == undefined) {
        calendar["prodID"] = "invalid file";
        calendar["version"] = "";
        calendar["numEvents"] = "";
        calendar["numProps"] = "";
      }
      calendar["filename"] = file;
      fileList.push(calendar);

    });

    return fileList;

}


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

  //console.log("TEST: " + test);
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


let connection;
app.post('/databaseLogin', function (req, res) {
  var string = req.body;
  console.log(string);

  //Connect to database
  connection = null;
  connection = mysql.createConnection({
    host     : 'dursley.socs.uoguelph.ca',
    user     : string.username,
    password : string.password,
    database : string.databaseName
  });
  connection.connect(function(err) {
  if (err)
    {
      console.log("Log In Error: " + err);

      res.send("{error}");
    }
    else {

    console.log("Connected!");
    let createTables = [
      "CREATE TABLE IF NOT EXISTS FILE ( cal_id INTEGER	AUTO_INCREMENT,file_Name VARCHAR(60) NOT NULL,version  INTEGER NOT NULL ,prod_id VARCHAR(256) NOT NULL ,PRIMARY KEY(cal_id))",
      "CREATE TABLE IF NOT EXISTS EVENT ( event_id INTEGER AUTO_INCREMENT ,summary VARCHAR(1024),start_time DATETIME NOT NULL,location VARCHAR(60),organizer VARCHAR(256),cal_file INTEGER NOT NULL,PRIMARY KEY(event_id) ,FOREIGN KEY(cal_file) REFERENCES FILE (cal_id) ON DELETE CASCADE)",
      "CREATE TABLE IF NOT EXISTS ALARM (alarm_id INTEGER AUTO_INCREMENT ,action VARCHAR(256) NOT NULL ,`trigger` VARCHAR(256) NOT NULL ,event INTEGER NOT NULL ,PRIMARY KEY(alarm_id) ,FOREIGN KEY(event) REFERENCES EVENT (event_id) ON DELETE CASCADE)"
    ];
    for (let table of createTables){
      connection.query(table, function (err, rows, fields) {
          if (err) console.log("Something went wrong. "+err);
          console.log("Create table query ran successfully!");
      });
    }
    //Get calendar data

    connection.query("SELECT COUNT(DISTINCT cal_id) as fileCount, COUNT(DISTINCT event_id) as eventCount, COUNT(DISTINCT alarm_id) as alarmCount FROM FILE, EVENT, ALARM", function (err, result, fields) {
      if (err) throw err;
      res.send(result[0]);
      console.log("Database files = "+ result[0]);

    });
    }
  });

  //Create Tables


});


app.post('/storeAllFiles', function (req, res) {
  var calendarListData = parseFLTableSync();
  console.log("Parsed successfully: " +  JSON.stringify(calendarListData));
  let index = 1;
  let eventIndex = 1;
  let fileCount = 0;

clearAllTables();

   for (let calendar of calendarListData){
     if (calendar["prodID"] == "invalid file") { continue;}
     let insertFileQuery = "INSERT IGNORE INTO FILE VALUES(NULL, " + "'"+ calendar.filename + "'"+ ", " +  calendar.version + ", " + "'"+calendar.prodID + "'" + ")";

     let selectFileQuery ="SELECT file_Name FROM FILE WHERE file_Name =" + "\'" + calendar.filename + "\'";
      fileCount++;
          connection.query(insertFileQuery, function (err, rows, fields) {
              if (err) console.log("Something went wrong. "+err);
              console.log("Insert file query ran successfully!");
          });

          var eventListData = geteventListData("uploads/" + calendar.filename);
          for(let theEvent of eventListData)
          {
            //let insertEventQuery = "INSERT IGNORE INTO EVENT VALUES(NULL, " + "'"+ calendar.filename + "'"+ ", " +  calendar.version + ", " + "'"+calendar.prodID + "'" + ")";
            console.log(theEvent);
            let summary;
            let datetime;
            let location;
            let organizer;
            let insertEventQuery =  "INSERT INTO EVENT VALUES(NULL, ";

            //INSERT SUMMARY
            if(theEvent.summary == '' || theEvent.summary == undefined){
              summary = null;
              insertEventQuery += "NULL,";
            }
            else {
              summary = theEvent.summary;
              insertEventQuery += "\'" + theEvent.summary+ "\'" + ",";
            }


            //INSERT DATETIME

            datetime = theEvent.startDT.date[0] + theEvent.startDT.date[1] + theEvent.startDT.date[2] + theEvent.startDT.date[3] + "-" + theEvent.startDT.date[4] + theEvent.startDT.date[5] + "-" + theEvent.startDT.date[6] + theEvent.startDT.date[7];
            datetime = datetime + " " + theEvent.startDT.time[0]  + theEvent.startDT.time[1] + ":"  + theEvent.startDT.time[2]  + theEvent.startDT.time[3] + ":" +  theEvent.startDT.time[4] +  theEvent.startDT.time[5];

            insertEventQuery += "\'"+  datetime + "\'" + ",";

            if(theEvent.properties.LOCATION == "" || theEvent.properties.LOCATION == undefined){
              location = null;
              insertEventQuery += "NULL,"
            }
            else{
              location = theEvent.properties.LOCATION ;
              insertEventQuery += "\'" + theEvent.properties.LOCATION+ "\'" + ",";
            }


            if(theEvent.properties.ORGANIZER == "" || theEvent.properties.ORGANIZER == undefined){
              insertEventQuery += "NULL,"
              organizer= null;
            }
            else{
              organizer = theEvent.properties.ORGANIZER ;
              insertEventQuery += "\'" + theEvent.properties.ORGANIZER+ "\'" + ",";
            }

            insertEventQuery += index + ")";
            console.log("INDEX: " + index);
            console.log(insertEventQuery);


            connection.query(insertEventQuery, function (err, rows, fields) {
                if (err) console.log("Something went wrong. "+err);
                console.log("Insert event query ran successfully!");
            });



            for(let alarm of theEvent.alarmList )
            {
              let insertAlarmQuery = "INSERT IGNORE INTO ALARM VALUES(NULL, " + "'"+ alarm.action + "'"+ ", " +  "'"+alarm.trigger +"'" + ", " + eventIndex  + ")";
              connection.query(insertAlarmQuery, function (err, rows, fields) {
                  if (err) console.log("Something went wrong. "+err);
                  console.log("Insert alarm query ran successfully!");
              });

              console.log(alarm);
            }

            eventIndex++;

          }



      index++;
    }



    connection.query("SELECT COUNT(DISTINCT cal_id) as fileCount, COUNT(DISTINCT event_id) as eventCount, COUNT(DISTINCT alarm_id) as alarmCount FROM FILE, EVENT, ALARM", function (err, result, fields) {
      if (err) throw err;
      res.send(result[0]);
      console.log("Database files = "+ result[0]);

    });

});

app.post('/clearAllData', function (req, res) {
  clearAllTables();
  connection.query("SELECT COUNT(DISTINCT cal_id) as fileCount, COUNT(DISTINCT event_id) as eventCount, COUNT(DISTINCT alarm_id) as alarmCount FROM FILE, EVENT, ALARM", function (err, result, fields) {
    if (err) throw err;
    res.send(result[0]);
    console.log("Database files = "+ result[0]);

  });

});

function clearAllTables()
{


    let deleteAlarmsQuery = "DELETE FROM ALARM";
    let deleteEventsQuery = "DELETE FROM EVENT";
    let deleteFilesQuery ="DELETE FROM FILE";
    let resetIncrementQuery ="ALTER TABLE FILE AUTO_INCREMENT = 1;"
      let resetIncrementQuery2 ="ALTER TABLE EVENT AUTO_INCREMENT = 1;"
      let resetIncrementQuery3 ="ALTER TABLE ALARM AUTO_INCREMENT = 1;"


    connection.query(deleteAlarmsQuery, function (err, result, fields) {

        if (err) console.log("Something went wrong. "+err);
        console.log("Deleted all alarm rows successfully!");

      });

    connection.query(deleteEventsQuery, function (err, result, fields) {

        if (err) console.log("Something went wrong. "+err);
        console.log("Deleted all event rows successfully!");

      });

    connection.query(deleteFilesQuery, function (err, result, fields) {

        if (err) console.log("Something went wrong. "+err);
        console.log("Deleted all file rows successfully!");

      });
      connection.query(resetIncrementQuery, function (err, result, fields) {

          if (err) console.log("Something went wrong. "+err);
          console.log("Auto increment was reset successfully!");

        });

        connection.query(resetIncrementQuery2, function (err, result, fields) {

            if (err) console.log("Something went wrong. "+err);
            console.log("Auto increment was reset successfully!");

          });
          connection.query(resetIncrementQuery3, function (err, result, fields) {

              if (err) console.log("Something went wrong. "+err);
              console.log("Auto increment was reset successfully!");

            });



}

app.get('/query1', function (req, res) {
  let query = "SELECT * FROM EVENT ORDER BY start_time";
  connection.query(query, function (err, result, fields) {

      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 1 succesfully executed");


    console.log(result);

    });
  //console.log(fileList);
});


app.get('/query2a', function (req, res) {
  let query = "select * from FILE";
  connection.query(query, function (err, result, fields) {

      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 2a succesfully executed");
    console.log(result);

    });
  //console.log(fileList);
});

app.post('/query2b', function (req, res) {
  var string = req.body;
  var num = string["cal_id"];
  let query = "select event_id, summary, start_time from EVENT WHERE cal_file =" + num;
  connection.query(query, function (err, result, fields) {

      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 2b succesfully executed");
    });
});


app.get('/query3a', function (req, res) {
  let query = "select * from EVENT where start_time in (     select start_time from EVENT group by start_time having count(*) > 1)order by start_time asc";

//  let query = "SELECT start_time, count(*) AS c FROM EVENT GROUP BY start_time HAVING c > 1 ORDER BY c DESC";
  connection.query(query, function (err, result, fields) {
      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 3a succesfully executed");

    });
  //console.log(fileList);
});

app.post('/query3b', function (req, res) {
  var string = req.body;
  var date = string["start_time"];
  console.log("DATE SENT: " + date);
  let query = "select * from EVENT WHERE start_time = '" + date + "'";
  connection.query(query, function (err, result, fields) {

      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 3b succesfully executed");

    });
});

app.get('/query4', function (req, res) {
  let query = " select * from EVENT join ALARM where event = event_id order by start_time";

//  let query = "SELECT start_time, count(*) AS c FROM EVENT GROUP BY start_time HAVING c > 1 ORDER BY c DESC";
  connection.query(query, function (err, result, fields) {
      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 4 succesfully executed");

    });
  //console.log(fileList);
});

app.get('/query5', function (req, res) {
  let query = " SELECT action, count(*) AS c FROM ALARM GROUP BY action HAVING c > 1 ORDER BY c DESC";

//  let query = "SELECT start_time, count(*) AS c FROM EVENT GROUP BY start_time HAVING c > 1 ORDER BY c DESC";
  connection.query(query, function (err, result, fields) {
      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 5 succesfully executed");

    });
  //console.log(fileList);
});


app.post('/query6a', function (req, res) {
  var string = req.body;
  var date = string["currentTime"];
  console.log("Current Time: " + date);
  let query = "select * from EVENT WHERE start_time < '" + date + "' order by start_time";
  connection.query(query, function (err, result, fields) {

      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 6a succesfully executed");

    });
});


app.post('/query6b', function (req, res) {
  var string = req.body;
  var date = string["currentTime"];
  console.log("Current Time: " + date);
  let query = "select * from EVENT WHERE start_time > '" + date + "' order by start_time";
  connection.query(query, function (err, result, fields) {

      if (err) console.log("Something went wrong. "+err);
      console.log(result);
      res.send(result);
      console.log("Query 6a succesfully executed");

    });
});




let c = "foo";

app.get('/ffiTest', function (req, res) {
  res.send({
    foo: c
  });
});
app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
