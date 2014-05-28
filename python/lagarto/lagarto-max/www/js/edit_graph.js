/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var monthNames = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];

// Database metadata
var database;

// Global array of columns associated to the graph
var arrColumns = new Array();

/**
 * Fill list of years
 */
function fillYears(field)
{
  for(var year=2012 ; year<2100 ; year++)
    field.options[field.options.length] = new Option(year, year);
}

/**
 * Fill list of months
 */
function fillMonths(field)
{
  for(var month=1 ; month<=12 ; month++)
  {
    var strMonth = "" + month;
    if (strMonth.length == 1)
      strMonth = "0" + strMonth;
    field.options[field.options.length] = new Option(monthNames[month-1], strMonth);
  }
}

/**
 * Fill list of days
 */
function fillDays(field)
{
  for(var day=1 ; day<=31 ; day++)
  {
    var strDay = "" + day;
    if (strDay.length == 1)
      strDay = "0" + strDay;

    field.options[field.options.length] = new Option(strDay, strDay);
  }
}

/**
 * Fill list of hours
 */
function fillHours(field)
{
  for(var hour=0 ; hour<24 ; hour++)
  {
    var strHour = "" + hour;
    if (strHour.length == 1)
      strHour = "0" + strHour;

    field.options[field.options.length] = new Option(strHour, strHour);
  }
}

/**
 * Fill list of minutes or seconds
 */
function fillMinSecs(field)
{
  for(var minsec=0 ; minsec<60 ; minsec++)
  {
    var strMinSec = "" + minsec;
    if (strMinSec.length == 1)
      strMinSec = "0" + strMinSec;

    field.options[field.options.length] = new Option(strMinSec, strMinSec);
  }
}

/**
 * Database data received
 */
function updateTables()
{
  // Fill select fields
  fillYears(document.getElementById("start_year"));
  fillMonths(document.getElementById("start_month"));
  fillDays(document.getElementById("start_day"));
  fillHours(document.getElementById("start_hour"));
  fillMinSecs(document.getElementById("start_minute"));
  fillMinSecs(document.getElementById("start_second"));
  fillYears(document.getElementById("end_year"));
  fillMonths(document.getElementById("end_month"));
  fillDays(document.getElementById("end_day"));
  fillHours(document.getElementById("end_hour"));
  fillMinSecs(document.getElementById("end_minute"));
  fillMinSecs(document.getElementById("end_second"));

  var jsonDoc = getJsonDoc();
  database = jsonDoc.database;

  var fldTables = document.getElementById("table");

  for(var table in database)
    fldTables.options[fldTables.options.length] = new Option(table, table);

  onchangeTable();
}

/**
 * DB table selected from drop-down list
 */
function onchangeTable()
{
  var tableName = document.getElementById("table").value;
  var fldEndpoint = document.getElementById("endp");

  // Empty list of endpoints
  fldEndpoint.options.length = 0;

  if (tableName != "")
  {
    if (tableName in database)
    {
      if ("endpoints" in database[tableName])
      {        
        for(var endpoint in database[tableName]["endpoints"])
        {
          var endpName = database[tableName]["endpoints"][endpoint];
          fldEndpoint.options[fldEndpoint.options.length] = new Option(endpName, endpName);
        }
      }
    }
  }
}

/**
 * Update table values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var graphs = jsonDoc.graphs;

  var graphName = "";
  if ("name" in getUrlVars())
  {
    graphName = getUrlVars()["name"];
    graphName = graphName.replace("%20", " ");
    if (graphName in graphs)
    {
      var config = graphs[graphName];
      document.getElementById("graphname").value = graphName;
      document.getElementById("table").value = config.table;
      onchangeTable();
      if ("title" in config)
        document.getElementById("title").value = config.title;
      if ("type" in config)
        document.getElementById("type").value = config.type;
      if ("samples" in config)
        document.getElementById("samples").value = config.samples;
      if ("start" in config)
      {
        document.getElementById("ena_start").checked = true;
        enableStartDate();
        document.getElementById("starttime").value = config.start;
        var dateTime = config.start.split(' ');
        var date = dateTime[0].split('-');
        document.getElementById("start_year").value = date[0];
        document.getElementById("start_month").value = date[1];
        document.getElementById("start_day").value = date[2];
        var time = dateTime[1].split(':');
        document.getElementById("start_hour").value = time[0];
        document.getElementById("start_minute").value = time[1];
        document.getElementById("start_second").value = time[2];
        
      }
      if ("end" in config)
      {
        document.getElementById("ena_end").checked = true;
        enableEndDate();
        document.getElementById("endtime").value = config.end;
        var dateTime = config.end.split(' ');
        var date = dateTime[0].split('-');
        document.getElementById("end_year").value = date[0];
        document.getElementById("end_month").value = date[1];
        document.getElementById("end_day").value = date[2];
        var time = dateTime[1].split(':');
        document.getElementById("end_hour").value = time[0];
        document.getElementById("end_minute").value = time[1];
        document.getElementById("end_second").value = time[2];
      }
      if ("miny" in config)
        document.getElementById("miny").value = config.miny;
      if ("maxy" in config)
        document.getElementById("maxy").value = config.maxy;
      if ("show_grid" in config)
      {
        if (config.show_grid.toLowerCase() == "on")        
          document.getElementById("show_grid").checked = true;
      }
      if ("columns" in config)
      {
        for(col in config["columns"])
          addColumn(config["columns"][col]);
      }
    }
  }
}

/**
 * Add column to the table
 */
function addColumnFromMenu()
{
  var table = document.getElementById("table").value;
  var endpoint = document.getElementById("endp").value;  

  addColumn(endpoint);
}

/**
 * Add column to the table
 */
function addColumn(endpoint)
{
  var dot = endpoint.lastIndexOf('.');
  if (dot == -1)
    return;
  var colName = endpoint.substring(dot+1);

  if (arrColumns.indexOf(endpoint) > -1)
  {
    alert(endpoint + " already included in graph");
    return;
  }

  var table = document.getElementById("coltable");
  var row = table.insertRow(table.rows.length);

  // Endpoint data
  cell = row.insertCell(0);
  cell.className = "cellitem";
  var data = document.createTextNode(endpoint);
  cell.appendChild(data);

  // Column name
  cell = row.insertCell(1);
  cell.className = "cellitem";
  var colname = document.createTextNode(colName);
  cell.appendChild(colname);


  // Save endpoint data in global array
  arrColumns.push(endpoint);
}

/**
 * Clear endpoint table
 */
function clearTable()
{
  var table = document.getElementById("coltable");

  // Clear table
  while (table.rows.length > 1)
    table.deleteRow(1);

  // Clear global array of endpoints
  arrColumns = [];
}

/**
 * Enable/disable starting date/time
 */
function enableStartDate()
{
  var enable = !document.getElementById("ena_start").checked;

  document.getElementById("start_year").disabled = enable;
  document.getElementById("start_month").disabled = enable;
  document.getElementById("start_day").disabled = enable;
  document.getElementById("start_hour").disabled = enable;
  document.getElementById("start_minute").disabled = enable;
  document.getElementById("start_second").disabled = enable;
}

/**
 * Enable/disable starting date/time
 */
function enableEndDate()
{
  var enable = !document.getElementById("ena_end").checked;

  document.getElementById("end_year").disabled = enable;
  document.getElementById("end_month").disabled = enable;
  document.getElementById("end_day").disabled = enable;
  document.getElementById("end_hour").disabled = enable;
  document.getElementById("end_minute").disabled = enable;
  document.getElementById("end_second").disabled = enable;
}
/**
 * Save form data
 */
function save()
{
  if(document.getElementById("ena_start").checked)
  {
    document.getElementById("starttime").value = document.getElementById("start_year").value + "-"
                                               + document.getElementById("start_month").value + "-"
                                               + document.getElementById("start_day").value + " "
                                               + document.getElementById("start_hour").value + ":"
                                               + document.getElementById("start_minute").value + ":"
                                               + document.getElementById("start_second").value;
  }

  if(document.getElementById("ena_end").checked)
  {
    document.getElementById("endtime").value =   document.getElementById("end_year").value + "-"
                                               + document.getElementById("end_month").value + "-"
                                               + document.getElementById("end_day").value + " "
                                               + document.getElementById("end_hour").value + ":"
                                               + document.getElementById("end_minute").value + ":"
                                               + document.getElementById("end_second").value;
  }

  document.getElementById("columns").value = arrColumns;

  document.getElementById("table_data_form").submit();
}

