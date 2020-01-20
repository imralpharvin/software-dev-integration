// Put all onload AJAX calls here, and event listeners
$(document).ready(function () {
  // On page-load AJAX Example
  $.ajax({
    type: 'get',            //Request type
    dataType: 'json',       //Data type - we will use JSON for almost everything
    url: '/someendpoint',   //The server endpoint we are connecting to
    success: function (data) {
      $('#blah').html("On page load, Received string '" + JSON.stringify(data) + "' from server");
      //We write the object to the console to show that the request was successful
      console.log(data);

    },
    fail: function (error) {
      // Non-200 return, do something with error
      console.log(error);
    }
  });

  initializeStatusPanel();
  getCalendarDataAndPopulate();
  console.log("BEFORE SUBMIT");
  submitFilenameAndPopulate();
  console.log("AFTER SUBMIT");
  var realButton = document.getElementById('realButton');
  var customText = document.getElementById('customText');

  let statusBoard = document.getElementById('statusBoard');
  document.getElementById('clearMessages').onclick = function () {
    statusBoard.innerHTML = "<div class=\"alert alert-primary alert-dismissible fade show \" role=\"alert\">Status Panel cleared.</div>";
  };

  document.getElementById('uploadButton').onclick = function () {
    realButton.click();
    console.log("REAL BUTTON CLICKED");

  };
  document.getElementById('realSubmit').onclick = function () {
    //$('#realSubmit').click();

    getCalendarDataAndPopulate();
    getEventListandPopulate();

  }

  document.getElementById('storeAllFiles').onclick = function () {
    //$('#realSubmit').click();

    $.ajax(
      {
        type: "POST",
        url: "./storeAllFiles",
        data: [],
        dataType: 'json',
        contentType: "application/json",
        success: function (data, no, yes) {
          console.log("Stored data sucessfully");
          console.log(data);
          statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">Stored data sucessfully. Database has "  + data.fileCount + " files, " + data.eventCount + " events, and " + data.alarmCount + " alarms.</div>";
          $("#statusBoard").scrollTop($('#statusBoard').height());
          window.location.hash = "statusBoard";

          $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/query2a',
            async: 'false',
            success: function (data) {
              populateQuery2DD(data);
            },
            fail: function (error) {
              // Non-200 return, do something with error
              console.log(error);
            }
          });

        },
        fail: function (error) {
          // Non-200 return, do something with error
          console.log(error);
        }
      });

  }

  document.getElementById('clearAllData').onclick = function () {
    //$('#realSubmit').click();

    $.ajax(
      {
        type: "POST",
        url: "./clearAllData",
        data: [],
        dataType: 'json',
        contentType: "application/json",
        success: function (data, no, yes) {
          console.log("Cleared all data sucessfully");
          console.log(data);
          statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">Cleared all data sucessfully. Database has "  + data.fileCount + " files, " + data.eventCount + " events, and " + data.alarmCount + " alarms.</div>";
          $("#statusBoard").scrollTop($('#statusBoard').height());
          window.location.hash = "statusBoard";

          $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/query2a',
            async: 'false',
            success: function (data) {
              populateQuery2DD(data);
            },
            fail: function (error) {
              // Non-200 return, do something with error
              console.log(error);
            }
          });

        },
        fail: function (error) {
          // Non-200 return, do something with error
          console.log(error);
        }
      });

  }

$('#databaseLogInForm').submit(function (event) {
    event.preventDefault();

    console.log("Database Submit Button Clicked.");
    var username = document.getElementById('username').value;
    var password = document.getElementById('password').value;
    var databaseName = document.getElementById('databaseName').value;

    console.log("Username: " + username);
    console.log("Password: " + password);
    console.log("Database Name: " + databaseName);

    let databaseLogin = {};
    databaseLogin["username"] = username;
    databaseLogin["password"] = password;
    databaseLogin["databaseName"] = databaseName;

    $.ajax(
      {
        type: "POST",
        url: "./databaseLogin",
        data: JSON.stringify(databaseLogin),
        dataType: 'json',
        contentType: "application/json",
        success: function (data, no, yes) {
          console.log("Database Log In Successful");
          console.log(data);
          var loginStatus = document.getElementById('loginStatus');
          loginStatus.innerHTML =  "<div class=\"alert alert-success \" role=\"alert\">Logged in successfully!</div>";

          statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">Logged in successfully. Database has "  + data.fileCount + " files, " + data.eventCount + " events, and " + data.alarmCount + " alarms.</div>";
          $("#statusBoard").scrollTop($('#statusBoard').height());


          var databaseButtons = document.getElementById('databaseButtons');
          databaseButtons.style.display = "block";

          $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/query2a',
            async: 'false',
            success: function (data) {
              populateQuery2DD(data);
            },
            fail: function (error) {
              // Non-200 return, do something with error
              console.log(error);
            }
          });



        },
        error: function (error, nan , nan) {
          // Non-200 return, do something with error
          var loginStatus = document.getElementById('loginStatus');
          loginStatus.innerHTML =  "<div class=\"alert alert-danger \" role=\"alert\">Log in error. Make sure you have the right username, password and database name.</div>";
          console.log(error);
        }
      });



  });

  document.getElementById('query1').onclick = function () {

    $.ajax({
      type: 'get',            //Request type
      dataType: 'json',       //Data type - we will use JSON for almost everything
      url: '/query1',   //The server endpoint we are connecting to
      success: function (data) {
        var statusQBoard = document.getElementById('statusQBoard');
        statusQBoard.innerHTML += "<hr><b>Query 1 Executed</b> <br>";
          statusQBoard.innerHTML += "<b>Displaying all events sorted by start date</b> <br>";
        for(theEvent of data)
        {
          var theDate = new Date(theEvent.start_time);
          statusQBoard.innerHTML += theDate;
          statusQBoard.innerHTML +=  " Event Id: " + theEvent.event_id + "<br>";
        }
        console.log(data);

      },
      fail: function (error) {
        // Non-200 return, do something with error
        console.log(error);
      }
    });

  }

  realButton.addEventListener("change", function () {
    var uploadFile = realButton.value;

    if (uploadFile != "invalid") {
      console.log("IF");
      var path = realButton.value
      var filename = path.replace(/^.*\\/, "");
      customText.innerHTML = filename;
    }
    else {
      console.log("else");
      customText.innerHTML = "No afile chosen";
    }
  });


  // Event listener form replacement example, building a Single-Page-App, no redirects if possible
  $('#someform').submit(function (e) {
    $('#blah').html("Callback from the form");
    e.preventDefault();
    //Pass data to the Ajax call, so it gets passed to the
    $.ajax({});
  });

  function initializeStatusPanel(){
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/calendarData',
      success: function (data) {
        var array = data;
        var statusBoard = document.getElementById('statusBoard');
        for(var i = 0; i < array.length; i++)
        {
          if(array[i].version != "")
          {
            statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">" + array[i].filename +   " is a valid file and has been added to the file log table.</div>";

          }
          else
          {
            statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-danger \" role=\"alert\">" +  array[i].filename + " is an invalid file.</div>";
          }
        }
        $("#statusBoard").scrollTop($('#statusBoard').height());

        console.log("FROM CALENDAR data: " + data);
        populateTables(data);
        console.log("getCalendarDataAndPopulate works");
      },
      fail: function (error) {
        // Non-200 return, do something with error
        console.log(error);
      }
    });
  }

  function getCalendarDataAndPopulate() {
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/calendarData',
      success: function (data) {
        console.log("FROM CALENDAR data: " + data);
        populateTables(data);
        console.log("getCalendarDataAndPopulate works");
      },
      fail: function (error) {
        // Non-200 return, do something with error
        console.log(error);
      }
    });
  }

  function updateCalendarDataAndPopulate() {
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/updateData',
      success: function (data) {
        console.log("UPDATE CALENDAR AND POPULATE" + data);
        populateTables(data);
        console.log("AFTER SUBMIT OR CREATE CALENDAR");
      },
      fail: function (error) {
        // Non-200 return, do something with error
        console.log(error);
      }
    });
  }

  function populateFLTable(data) {

    var tempArray = data;
    var array = [];
    for(var i = 0; i < tempArray.length; i++)
    {
      if(tempArray[i]["version"] != "")
      {
        array.push(tempArray[i]);
      }
    }

    if (array.length == 0) {
      console.log("here" + array.length);
      $('#noFiles').html("No files");
      $("#storeAllFiles").hide();
    }
    else {
      $("#storeAllFiles").show();

      let table = document.getElementById('fileLogTable');

      table.innerHTML = "";

      var row = table.insertRow(-1);
      for (var i = 0; i < 5; i++) {
        var headerCell = document.createElement("TH");
        row.appendChild(headerCell);
      }


      row.cells[0].innerHTML = "File name (click to download)";
      row.cells[1].innerHTML = "Version";
      row.cells[2].innerHTML = "Product ID";
      row.cells[3].innerHTML = "Number of events";
      row.cells[4].innerHTML = "Number of properties";


      for (var i = 1; i <= array.length; i++) {
        row = table.insertRow(-1);
        for (var j = 0; j < 5; j++) {
          var cell = row.insertCell(-1);
        }

        console.log(array[i - 1]["filename"]);
        table.rows[i].cells[0].innerHTML = "<a href=\"/uploads/" + array[i - 1]["filename"] + "\">" + array[i - 1]["filename"] + "</a>";
        table.rows[i].cells[1].innerHTML = array[i - 1]["version"];
        table.rows[i].cells[2].innerHTML = array[i - 1]["prodID"];
        table.rows[i].cells[3].innerHTML = array[i - 1]["numEvents"];
        table.rows[i].cells[4].innerHTML = array[i - 1]["numProps"];
      }
    }
  }

  function populateDropDown(data) {
    var array = data;
    var option = document.getElementById('dropDown');

    var sel = document.getElementById('dropDown');
    sel.innerHTML = "";
    var opt = document.createElement('option');
    //opt.appendChild( document.createTextNode('No');
    opt.value = "";
    sel.appendChild(opt);

    for (var i = 0; i < array.length; i++) {
      if (array[i]["prodID"] != "invalid file") {
        var sel = document.getElementById('dropDown');
        var opt = document.createElement('option');
        opt.appendChild(document.createTextNode(array[i]["filename"]));
        opt.value = array[i]["filename"];
        sel.appendChild(opt);
      }
    }





  }

  function populateTables(data) {
    populateFLTable(data);
    populateDropDown(data);
  }

  function getEventListandPopulate() {
    $.ajax({
      type: 'get',
      dataType: 'json',
      url: '/eventListData',
      async: 'false',
      success: function (data) {
        populateEventList(data);
      },
      fail: function (error) {
        // Non-200 return, do something with error
        console.log(error);
      }
    });
  }

  function populateEventList(data) {
    let array = data;
    console.log("POPULATE EVNT " + array);
    var option = document.getElementById('dropDown');
    var filename = option.value;

    var show = document.getElementById('calendarShow');

    if (filename == "") {
      show.style.display = 'none';           // Hide

    }
    else {
      show.style.display = 'block';
    }

    let table = document.getElementById('eventListTable');
    while (table.rows.length > 0) {
      table.deleteRow(0);
    }
    var row = table.insertRow(-1);
    for (var i = 0; i < 6; i++) {
      var headerCell = document.createElement("TH");
      row.appendChild(headerCell);
    }

    row.cells[0].innerHTML = "Event No";
    row.cells[1].innerHTML = "Start date";
    row.cells[2].innerHTML = "Start time";
    row.cells[3].innerHTML = "Summary";
    row.cells[4].innerHTML = "Props";
    row.cells[5].innerHTML = "Alarms";

    for (var i = 1; i <= array.length; i++) {
      row = table.insertRow(-1);
      for (var j = 0; j < 6; j++) {
        var cell = row.insertCell(-1);
      }

      var startDate = array[i - 1]["startDT"]["date"];
      var startTime = array[i - 1]["startDT"]["time"];

      "<span class=\"badge badge-secondary\">UTC</span>"


      table.rows[i].cells[0].innerHTML = i;
      table.rows[i].cells[1].innerHTML = startDate[0] + startDate[1] + startDate[2] + startDate[3] + "/" + startDate[4] + startDate[5] + "/" + startDate[6] + startDate[7];
      if(array[i-1]["startDT"]["isUTC"] == true)
      {
        table.rows[i].cells[1].innerHTML += " <span class=\"badge badge-secondary\">UTC</span>";
      }
      table.rows[i].cells[2].innerHTML = startTime[0] + startTime[1] + ":"+ startTime[2] + startTime[3] + ":" + startTime[4] + startTime[5];
      table.rows[i].cells[3].innerHTML = array[i - 1]["summary"];
      table.rows[i].cells[4].innerHTML = array[i - 1]["numProps"];
      table.rows[i].cells[5].innerHTML = array[i - 1]["numAlarms"];
    }
    console.log(data);

    //Drop down eventToJSON


    var dropDownEvent = document.getElementById('dropDownEvent');

    for (var i = dropDownEvent.options.length - 1; i >= 0; i--) {
      dropDownEvent.remove(i);
    }

    var opt = document.createElement('option');
    opt.appendChild(document.createTextNode(""));
    opt.value = "Select Event";
    dropDownEvent.appendChild(opt);

    for (var i = 0; i < array.length; i++) {

      var opt = document.createElement('option');
      opt.appendChild(document.createTextNode("Event No " + (i + 1)));
      opt.value = JSON.stringify(array[i]["alarmList"]);
      console.log(JSON.stringify(array[i]["alarmList"]));
      console.log(array[i]["alarmList"]);
      dropDownEvent.appendChild(opt);
    }

    var dropDownProperties = document.getElementById('dropDownProperties');

    for (var i = dropDownProperties.options.length - 1; i >= 0; i--) {
      dropDownProperties.remove(i);
    }

    var opt = document.createElement('option');
    opt.appendChild(document.createTextNode(""));
    opt.value = "Select Event";
    dropDownProperties.appendChild(opt);

    for (var i = 0; i < array.length; i++) {

      var opt = document.createElement('option');
      opt.appendChild(document.createTextNode("Event No " + (i + 1)));
      opt.value = JSON.stringify(array[i]["properties"]);
      console.log(JSON.stringify(array[i]["properties"]));
      console.log(array[i]["properties"]);
      dropDownProperties.appendChild(opt);
    }
  }

  var option = document.getElementById('dropDown');
  option.addEventListener("change", function () {
    submitFilenameAndPopulate();
  });

  function submitFilenameAndPopulate() {
    console.log("DURING: " + JSON.stringify(string));
    var option = document.getElementById('dropDown');

    var selectedVal = option.value;
    console.log("DURING2: " + JSON.stringify(string));

    var string = {};
    string["filename"] = selectedVal;
    console.log("STRING: " + JSON.stringify(string));
    console.log("CHANGE: " + string["filename"]);

    $.ajax(
      {
        type: "POST",
        url: "./eventFilename",
        data: JSON.stringify(string),
        dataType: 'json',
        contentType: "application/json",
        success: function (data, no, yes) {
          console.log("SUCCESSS POST" + data["filename"]);

          getEventListandPopulate();
          console.log("YESSSS");
        },
        fail: function (error, yes, no) {
          // Non-200 return, do something with error
          getEventListandPopulate();
          console.log("NO");
        }
      });

    console.log("AFTER POST" + selectedVal);
  }

  var dropDownEvent = document.getElementById('dropDownEvent');
  dropDownEvent.addEventListener("change", function () {
    var opt = dropDownEvent.options[dropDownEvent.selectedIndex];
    var alarmList = document.getElementById('alarmList');

    alarmList.innerHTML = "";
    if (opt.value == "") {
      alarmList.innerHTML = "";
    }

    else {


      var array = JSON.parse(opt.value);
      if (array.length < 1) {
        alarmList.innerHTML = "No alarms for this event." + "<br>";
      }
      else {
        for (i = 0; i < array.length; i++) {
          alarmList.innerHTML = alarmList.innerHTML + "Alarm No " + (i + 1) + "<br>";
          console.log(array[i]);
          var action = array[i]["action"];
          var trigger = array[i]["trigger"];
          var numProps = array[i]["numProps"];

          alarmList.innerHTML = alarmList.innerHTML + "Action: " + action + "<br>";
          alarmList.innerHTML = alarmList.innerHTML + "Trigger: " + trigger + "<br>";
          alarmList.innerHTML = alarmList.innerHTML + "Number of Properties: " + numProps + "<br><br>";

        }

        statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">" + alarmList.innerHTML +   "</div>";

        $("#statusBoard").scrollTop($('#statusBoard').height());
        window.location.hash = "statusBoard";

      }
    }

  });

  var dropDownProperties = document.getElementById('dropDownProperties');
  var status = document.getElementById('status');

  dropDownProperties.addEventListener("change", function () {
    var opt = dropDownProperties.options[dropDownProperties.selectedIndex];
    var propertyList = document.getElementById('propertyList');

    propertyList.innerHTML = "";
    if (opt.value == "Select Event") {
      propertyList.innerHTML = "";
    }

    else {


      var jsonArray = JSON.parse(opt.value);
      console.log("HERE :" + array);
      var array = [];
      for (var k in jsonArray) {
        array.push(k);
        console.log(k);
      }

      if (array.length < 1) {
        propertyList.innerHTML = "No properties for this event." + "<br>";
        //$("#statusBoard").scrollTop($('#statusBoard').height());

      }
      else {
        for (i = 0; i < array.length; i++) {
          propertyList.innerHTML = propertyList.innerHTML + array[i] + " : " + jsonArray[array[i]] + "<br>";


        }
        statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">" + propertyList.innerHTML +   "</div>";
        $("#statusBoard").scrollTop($('#statusBoard').height());
        window.location.hash = "statusBoard";

      }
    //  $("#statusBoard").scrollTop($('#statusBoard').height());
    }
  });

  var createCalendarForm = document.getElementById('createCalendarForm');
  $('#createCalendarForm').submit(function (event) {

    /* stop form from submitting normally */
    event.preventDefault();
    var uid = document.getElementById('uid').value;
    var filename = document.getElementById('filename').value;
    var version = document.getElementById('version').value;
    var prodID = document.getElementById('prodID').value;
    var startDate = document.getElementById('startDate').value;
    var startTime = document.getElementById('startTime').value;
    var utc = (document.getElementById('UTC').value == "true" ? true : false);

    console.log(uid);
    console.log(filename);
    console.log(version);
    console.log(prodID);
    console.log(startDate);
    console.log(startTime);
    console.log(utc);

    var today = new Date();
    var date = today.getFullYear() + '-' + today.getMonth() + 1 + '-' + today.getDate();
    var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
    var timeZone = today.getTimezoneOffset();
    var dateTime = date + ' ' + time;

    console.log("DATE:" + dateTime + " " + timeZone);

    console.log("FILENAME: " + filename);

    var calendar = {};

    calendar["filename"] = filename + ".ics";
    calendar["version"] = version;
    calendar["prodID"] = prodID;
    calendar["UID"] = uid;

    calendar["startDate"] = startDate[0] + startDate[1] + startDate[2] + startDate[3] + startDate[5] + startDate[6] + startDate[8] + startDate[9];
    console.log(calendar["startDate"]);
    calendar["startTime"] = startTime[0] + startTime[1] + startTime[3] + startTime[4] + "00";
    console.log(calendar["startTime"]);
    calendar["startUTC"] = utc;
    console.log(calendar["startUTC"]);

    var createMonth = ((today.getMonth() + 1) < 10 ? '0' : '') + (today.getMonth() + 1);
    var createDay = (today.getDate() < 10 ? '0' : '') + today.getDate();
    var createHour = (today.getHours() < 10 ? '0' : '') + today.getHours();
    var createMinute = (today.getMinutes() < 10 ? '0' : '') + today.getMinutes();
    var createSeconds = (today.getSeconds() < 10 ? '0' : '') + today.getSeconds();

    var createUTC = (today.getTimezoneOffset() == 0 ? true : false);

    calendar["createDate"] = (today.getFullYear()).toString() + createMonth + createDay;
    console.log(calendar["createDate"]);

    calendar["createTime"] = createHour + createMinute + createSeconds;
    console.log(calendar["createTime"]);
    console.log(calendar);
    calendar["createUTC"] = createUTC;

    statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">" + calendar.filename +   " has been created and added to the file log table.</div>";
    $("#statusBoard").scrollTop($('#statusBoard').height());

    $.ajax(
      {
        type: "POST",
        url: "./createCalendarInfo",
        data: JSON.stringify(calendar),
        dataType: 'json',
        contentType: "application/json",
        success: function (data, no, yes) {
          console.log("SUCCESSS POST" + data["filename"]);

          getCalendarDataAndPopulate();
          getEventListandPopulate();
          /*console.log("YESSSS");*/
        },
        fail: function (error) {
          // Non-200 return, do something with error
          console.log(error);
          getEventListandPopulate();
          console.log("NO");
        }
      });






  });

  $('#addEventForm').submit(function (event) {
    event.preventDefault();

    var option = document.getElementById('dropDown');
    var filename = option.value;
    console.log("ADD EVENT:" + option.value);

    var uid = document.getElementById('addUID').value;
    var startDate = document.getElementById('addStartDate').value;
    var startTime = document.getElementById('addStartTime').value;
    var utc = (document.getElementById('addUTC').value == "true" ? true : false);

    console.log(uid);
    console.log(startDate);
    console.log(startTime);
    console.log(utc);

    var today = new Date();
    var date = today.getFullYear() + '-' + today.getMonth() + 1 + '-' + today.getDate();
    var time = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
    var timeZone = today.getTimezoneOffset();
    var dateTime = date + ' ' + time;

    console.log("DATE:" + dateTime + " " + timeZone);

    console.log("FILENAME: " + filename);

    var eventInfo = {};

    eventInfo["filename"] = filename;
    eventInfo["UID"] = uid;

    eventInfo["startDate"] = startDate[0] + startDate[1] + startDate[2] + startDate[3] + startDate[5] + startDate[6] + startDate[8] + startDate[9];
    console.log(eventInfo["startDate"]);
    eventInfo["startTime"] = startTime[0] + startTime[1] + startTime[3] + startTime[4] + "00";
    console.log(eventInfo["startTime"]);
    eventInfo["startUTC"] = utc;
    console.log(eventInfo["startUTC"]);

    var createMonth = ((today.getMonth() + 1) < 10 ? '0' : '') + (today.getMonth() + 1);
    var createDay = (today.getDate() < 10 ? '0' : '') + today.getDate();
    var createHour = (today.getHours() < 10 ? '0' : '') + today.getHours();
    var createMinute = (today.getMinutes() < 10 ? '0' : '') + today.getMinutes();
    var createSeconds = (today.getSeconds() < 10 ? '0' : '') + today.getSeconds();

    var createUTC = (today.getTimezoneOffset() == 0 ? true : false);

    eventInfo["createDate"] = (today.getFullYear()).toString() + createMonth + createDay;
    console.log(eventInfo["createDate"]);

    eventInfo["createTime"] = createHour + createMinute + createSeconds;
    console.log(eventInfo["createTime"]);
    console.log(eventInfo);
    eventInfo["createUTC"] = createUTC;



    $.ajax(
      {
        type: "POST",
        url: "./addEventInfo",
        data: JSON.stringify(eventInfo),
        dataType: 'json',
        contentType: "application/json",
        success: function (data, no, yes) {
          console.log("SUCCESSS ADD EVENT POST" + data["filename"]);

          getCalendarDataAndPopulate();
          getEventListandPopulate();
          statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">" +   "An event has been created and added to the" + eventInfo["filename"] +"calendar.</div>";
          $("#statusBoard").scrollTop($('#statusBoard').height());
          /*console.log("YESSSS");*/
        },
        fail: function (error) {
          // Non-200 return, do something with error
          console.log(error);
          getEventListandPopulate();
          console.log("NO");
        }
      });

  });

  var dropDownEvent = document.getElementById('dropDownEvent');
  dropDownEvent.addEventListener("change", function () {
    var opt = dropDownEvent.options[dropDownEvent.selectedIndex];
    var alarmList = document.getElementById('alarmList');

    alarmList.innerHTML = "";
    if (opt.value == "") {
      alarmList.innerHTML = "";
    }

    else {


      var array = JSON.parse(opt.value);
      if (array.length < 1) {
        alarmList.innerHTML = "No alarms for this event." + "<br>";
      }
      else {
        for (i = 0; i < array.length; i++) {
          alarmList.innerHTML = alarmList.innerHTML + "Alarm No " + (i + 1) + "<br>";
          console.log(array[i]);
          var action = array[i]["action"];
          var trigger = array[i]["trigger"];
          var numProps = array[i]["numProps"];

          alarmList.innerHTML = alarmList.innerHTML + "Action: " + action + "<br>";
          alarmList.innerHTML = alarmList.innerHTML + "Trigger: " + trigger + "<br>";
          alarmList.innerHTML = alarmList.innerHTML + "Number of Properties: " + numProps + "<br><br>";

        }

        statusBoard.innerHTML = statusBoard.innerHTML + "<div class=\"alert alert-success \" role=\"alert\">" + alarmList.innerHTML +   "</div>";

        $("#statusBoard").scrollTop($('#statusBoard').height());
        window.location.hash = "statusBoard";
      }
    }
  });

function populateQuery2DD(data)
{
  var array = data;
  var option = document.getElementById('query2DD');

  var sel = document.getElementById('query2DD');
  sel.innerHTML = "";
  var opt = document.createElement('option');
  //opt.appendChild( document.createTextNode('No');
  opt.value = "";
  sel.appendChild(opt);


  for (var i = 0; i < array.length; i++) {

      var sel = document.getElementById('query2DD');
      var opt = document.createElement('option');
      opt.appendChild(document.createTextNode(array[i]["file_Name"]));
      opt.value = array[i]["cal_id"];
      sel.appendChild(opt);

  }
}

var dropDownQ2 = document.getElementById('query2DD');
dropDownQ2.addEventListener("change", function () {
  var opt = dropDownQ2.options[dropDownQ2.selectedIndex];
  var statusQBoard = document.getElementById('statusQBoard');

  statusQBoard.innerHTML += "";
  if (opt.value == "") {
    statusQBoard.innerHTML += "";
  }

  else {
    //var array = JSON.parse(opt.value);

    num = {};
    num['cal_id'] = opt.value;

    $.ajax(
      {
        type: "POST",
        url: "./query2b",
        data: JSON.stringify(num),
        dataType: 'json',
        contentType: "application/json",
        success: function (data, no, yes) {
          console.log("Query 2b Successful");
          console.log(data);
          statusQBoard.innerHTML += "<hr><b>Query 2 Executed</b> <br>";
            statusQBoard.innerHTML += "<b>Displaying all events from " + opt.text + "</b> <br>";

          for(theEvent of data)
          {
            var theDate = new Date(theEvent.start_time);
            statusQBoard.innerHTML += theDate + "<br>";
            statusQBoard.innerHTML +=  "Summary: " + theEvent.summary + "<br><br>";
          }

        },
        fail: function (error) {
          console.log(error);
        }
      });
  }

});

document.getElementById('query3').onclick = function () {

  $.ajax({
    type: 'get',            //Request type
    dataType: 'json',       //Data type - we will use JSON for almost everything
    url: '/query3a',   //The server endpoint we are connecting to
    success: function (data) {
      var statusQBoard = document.getElementById('statusQBoard');
      statusQBoard.innerHTML += "<hr><b>Query 3 Executed</b> <br>";
        statusQBoard.innerHTML += "<b>Displaying all conflicting events</b> <br>";


    var previous;
      for(theEvent of data)
      {

        var theDate = new Date(theEvent.start_time);
        if(previous - theDate != 0)
        {
          statusQBoard.innerHTML +=  "<br>";
        }
        statusQBoard.innerHTML += "<b>"+theDate + "</b>" + "<br>";
        if(theEvent.summary!= null)
        {
        statusQBoard.innerHTML +=  "Summary: " + theEvent.summary + "<br>";
        }
        else {
          statusQBoard.innerHTML +=  "Summary: No summary<br>";
        }

        if(theEvent.organizer!= null)
        {
        statusQBoard.innerHTML +=  "Organizer: " + theEvent.organizer + "<br>";
      }
      else {
        statusQBoard.innerHTML +=  "Organizer: No organizer<br>";
      }

        previous = theDate;
      }



    },
    fail: function (error) {
      // Non-200 return, do something with error
      console.log("Query 3: "+ error);
    }
  });

}

document.getElementById('query4').onclick = function () {

  $.ajax({
    type: 'get',            //Request type
    dataType: 'json',       //Data type - we will use JSON for almost everything
    url: '/query4',   //The server endpoint we are connecting to
    success: function (data) {
      var statusQBoard = document.getElementById('statusQBoard');

      statusQBoard.innerHTML += "<hr><b>Query 4 Executed</b> <br>";
        statusQBoard.innerHTML += "<b>Displaying all alarms and their corresponding events</b> <br>";

      for(theAlarm of data)
      {
        var theDate = new Date(theAlarm.start_time);
        statusQBoard.innerHTML += "Alarm ID: " +theAlarm.alarm_id + "<br>";
        statusQBoard.innerHTML += "Action: " +theAlarm.action + "<br>";
        statusQBoard.innerHTML += "Trigger: "+ theAlarm.trigger + "<br>";
        statusQBoard.innerHTML += "Date: " +theDate + "<br><br>";
      }



    },
    fail: function (error) {
      // Non-200 return, do something with error
      console.log("Query 4: "+ error);
    }
  });

}

document.getElementById('query5').onclick = function () {

  $.ajax({
    type: 'get',            //Request type
    dataType: 'json',       //Data type - we will use JSON for almost everything
    url: '/query5',   //The server endpoint we are connecting to
    success: function (data) {
      statusQBoard.innerHTML += "<hr><b>Query 5 Executed</b> <br>";
        statusQBoard.innerHTML += "<b>Displaying count of types of alarm actions</b> <br>";

      for(theAlarm of data)
      {
        statusQBoard.innerHTML +=  theAlarm.action + ": "+ theAlarm.c + "<br>";
      }

    },
    fail: function (error) {
      // Non-200 return, do something with error
      console.log("Query 5: "+ error);
    }
  });

}

document.getElementById('query6a').onclick = function () {
  var dateobj = new Date();

  var B = dateobj.toISOString();

  var date = {};
  date["currentTime"] = B;

  $.ajax(
    {
      type: "POST",
      url: "./query6a",
      data: JSON.stringify(date),
      dataType: 'json',
      contentType: "application/json",
      success: function (data, no, yes) {
        console.log("Query 6a Successful");
        console.log(data);
        statusQBoard.innerHTML += "<hr><b>Query 6 Executed</b> <br>";
          statusQBoard.innerHTML += "<b>Displaying all past events</b> <br>";

        for(theEvent of data)
        {
          var theDate = new Date(theEvent.start_time);
          statusQBoard.innerHTML += "<b>"+theDate + "</b>" + "<br>";
          if(theEvent.summary!= null)
          {
          statusQBoard.innerHTML +=  "Summary: " + theEvent.summary + "<br>";
          }
          else {
            statusQBoard.innerHTML +=  "Summary: No summary<br>";
          }

          if(theEvent.organizer!= null)
          {
          statusQBoard.innerHTML +=  "Organizer: " + theEvent.organizer + "<br>";
        }
        else {
          statusQBoard.innerHTML +=  "Organizer: No organizer<br>";
        }


        }
        //statusQBoard.innerHTML += JSON.stringify(data);
      },
      fail: function (error) {
        // Non-200 return, do something with error
        console.log(error);
      }
    });
}

document.getElementById('query6b').onclick = function () {
  var dateobj = new Date();

  var B = dateobj.toISOString();

  var date = {};
  date["currentTime"] = B;

  $.ajax(
    {
      type: "POST",
      url: "./query6b",
      data: JSON.stringify(date),
      dataType: 'json',
      contentType: "application/json",
      success: function (data, no, yes) {
        console.log("Query 6b Successful");
        console.log(data);
        statusQBoard.innerHTML += "<hr><b>Query 6 Executed</b> <br>";
          statusQBoard.innerHTML += "<b>Displaying all future events</b> <br>";

        for(theEvent of data)
        {
          var theDate = new Date(theEvent.start_time);
          statusQBoard.innerHTML += "<b>"+theDate + "</b>" + "<br>";
          if(theEvent.summary!= null)
          {
          statusQBoard.innerHTML +=  "Summary: " + theEvent.summary + "<br>";
          }
          else {
            statusQBoard.innerHTML +=  "Summary: No summary<br>";
          }

          if(theEvent.organizer!= null)
          {
          statusQBoard.innerHTML +=  "Organizer: " + theEvent.organizer + "<br>";
        }
        else {
          statusQBoard.innerHTML +=  "Organizer: No organizer<br>";
        }


        }
      },
      fail: function (error) {
        // Non-200 return, do something with error
        console.log(error);
      }
    });
}


});
