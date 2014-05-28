/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var monthNames = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];

var graphName, tableName;
var minYaxis = 0 , maxYaxis = 0;
var showGrid = false;
var typeOfGraph = "line";
var title = "";
var updateInterval;

/**
 * Query database
 */
function updateValues()
{ 
  // Load graph settings
  var jsonDoc = getJsonDoc();
  var graphs = jsonDoc.graphs;

  // Check graph name
  if (!(graphName in graphs))
  {
    alert("Graph name not found in graphs.json");
    return;
  }

  var graph = graphs[graphName];

  if ("title" in graph)
    title = graph["title"];

  if ("type" in graph)
    typeOfGraph = graph["type"];

  if ("miny" in graph)
    minYaxis = graph["miny"];

  if ("maxy" in graph)
    maxYaxis = graph["maxy"];

  if ("show_grid" in graph)
    showGrid = (graph["show_grid"].toLowerCase() == "on");

  tableName = graph.table;
  var query = "/command/query_db_table?table=" + tableName;

  if ("columns" in graph)
  {
    var columnNames = new Array();
    for(col in graph.columns)
    {
      var dot = graph.columns[col].lastIndexOf('.');
      if (dot == -1)
        return;

      columnNames.push(graph.columns[col].substring(dot+1));
    }
    query += "&columns=" + columnNames;
  }
  if ("start" in graph)
    query += "&start=" + graph.start;
  if ("end" in graph)
    query += "&end=" + graph.end;
  if ("samples" in graph)
    query += "&samples=" + graph.samples;

  // Get update interval
  loadJSONdata("/config/database.json", getUpdateInterval);
  // Query server
  loadJSONdata(query, plotGraph);
}

/**
 * Draw graph
 */
function plotGraph()
{
  // Load data
  var jsonDoc = getJsonDoc();
  var columns = jsonDoc.columns
  var data = jsonDoc.data;
  var i, j;

  if (title == "")
    appletsource = "";
  else
    appletsource = "<h1>" + title + "</h1>\n";
  appletsource += "<applet id='plotapp' code='jcckit.PlotApplet' archive='/JCCKit/jcckit.jar' width=500 height=300 >\n";
  appletsource += "<param name='background' value='0xffffff'>";
  appletsource += "<param name='data/curves' value='" + columns.slice(1, columns.length).join(' ') + "'>\n";

  var xData = new Array();
  var xLabel = new Array();
  var minX = 0, maxX = data.length-1;

  // Convert date/times in time deltas (seconds)
  for(i=0 ; i<data.length ; i++)
  {
    xData[i] = i;
    xLabel[i] = xData[i] + "=" + getTicLabel(data[i][0]);
  }

  // Type of graph
  switch(typeOfGraph)
  {
    case "bars":
      appletsource += "<param name='defaultCurveDefinition/symbolFactory/className' value='jcckit.plot.BarFactory'>";
      appletsource += "<param name='defaultCurveDefinition/symbolFactory/size' value='0.06'>";
      appletsource += "<param name='defaultCurveDefinition/symbolFactory/attributes/className' value='jcckit.graphic.BasicGraphicAttributes'>";
      appletsource += "<param name='plot/curveFactory/definitions' value='" + columns.slice(1, columns.length).join(' ') + "'>";
      appletsource += "<param name='defaultCurveDefinition/withLine' value='false'>";

      appletsource += "<param name='plot/initialHintForNextCurve/className' value='jcckit.plot.PositionHint'>";
      appletsource += "<param name='plot/initialHintForNextCurve/position' value='-0.01 0.1'>";

      appletsource += "<param name='plot/coordinateSystem/xAxis/minimum' value='" + (minX - xData[1]/2) + "'>\n";
      appletsource += "<param name='plot/coordinateSystem/xAxis/maximum' value='" + (maxX + xData[1]/2) + "'>\n";
      break;
    default:
      appletsource += "<param name='plot/coordinateSystem/xAxis/minimum' value='" + minX + "'>\n";
      appletsource += "<param name='plot/coordinateSystem/xAxis/maximum' value='" + maxX + "'>\n";
      break;
  }

  // Max/min values - Y axis
  if (minYaxis != maxYaxis)
  {
    appletsource += "<param name='plot/coordinateSystem/yAxis/minimum' value='" + minYaxis + "'>\n";
    appletsource += "<param name='plot/coordinateSystem/yAxis/maximum' value='" + maxYaxis + "'>\n";
  }

  // Axis
  appletsource += "<param name='plot/coordinateSystem/xAxis/axisLabel' value=''>\n";
  appletsource += "<param name='plot/coordinateSystem/yAxis/axisLabel' value=''>\n";

  // Time axis
  appletsource += "<param name='data/common/x' value='" + xData.join(' ') + "'>\n";
  appletsource += "<param name='plot/coordinateSystem/xAxis/ticLabelFormat/className', value='jcckit.plot.TicLabelMap'>\n";
  appletsource += "<param name='plot/coordinateSystem/xAxis/ticLabelFormat/map' value='" + xLabel.join(';') + "'>\n";

  // For each column
  for(i=1 ; i<columns.length ; i++)
  {
    // Bar color
    appletsource += "<param name='plot/curveFactory/" + columns[i] + "/symbolFactory/attributes/fillColor' value='" + getColor(i-1) + "'>";
    // Endpoint values
    appletsource += "<param name='data/" + columns[i] + "/' value='data/common/'>\n";
    appletsource += "<param name='data/" + columns[i] + "/title' value='" + columns[i] + "'>\n";

    var yData = new Array();
    for(j in data)
      yData[j] = data[j][i]

    appletsource += "<param name='data/" + columns[i] + "/y' value='" + yData.join(' ') + "'>\n";

    // Curve properties
    appletsource += "<param name='plot/curveFactory/" + columns[i] + "/' value='defaultCurveDefinition/'>";
  }

  // Legend
  appletsource += "<param name='plot/legend/titleAttributes/fontStyle' value='bold'>";
  appletsource += "<param name='plot/legend/titleAttributes/fontSize' value='0.02'>";
  appletsource += "<param name='plot/legend/curveTitleAttributes/fontStyle' value='italic'>";
  appletsource += "<param name='plot/legend/curveTitleAttributes/fontSize' value='0.02'>";

  if (showGrid)
  {
    // Show grid
    appletsource += "<param name='defaultCoordinateSystem/grid' value='true'>";
    appletsource += "<param name='defaultCoordinateSystem/ticLength' value='0'>";
    appletsource += "<param name='plot/coordinateSystem/xAxis/' value='defaultCoordinateSystem/'>";
    appletsource += "<param name='plot/coordinateSystem/yAxis/' value='defaultCoordinateSystem/'>";
  }

  // End of applet
  appletsource += "<applet/>";

  // Setting applet to load withing div tags
  document.getElementById("appletplace").innerHTML = appletsource;
}

/**
 * Return a line/bar color code given an index
 */
function getColor(index)
{
  var colors = [0x500000,  // Red
                0x000050,  // Blue
                0x005000,  // Green,
                0xefc810,  // Yellow
                0x5b416b,  // Purple
                0xd44124,  // Orange
                0xd441b2,  // Pink
                0x38a0c8,  // Cyan
                0x5b4124,  // Brown
                0x53c838]  // Light green

  if (index < colors.length)
    return colors[index];
  else
    return (0x505050 * 0x50 * (index - colors.length + 1))
}

/**
 * Get update interval for the current graph
 */
function getUpdateInterval()
{
  // Load table settings
  var jsonDoc = getJsonDoc();
  var database = jsonDoc.database;

  // Check graph name
  if (!(tableName in database))
  {
    alert("Table name not found in database.json");
    return;
  }

  var table = database[tableName];
  updateInterval = table.interval;
}

/**
 * Return X label for the date passed as argument
 */
function getTicLabel(date)
{
  if (updateInterval.indexOf('M') != -1)
  {
    var d = new Date(date);
    return monthNames[d.getMonth()];
  }
  else if (updateInterval.indexOf('d') != -1 || updateInterval.indexOf('w') != -1)
    return date.split(' ')[0];
  else
  {
    var time = date.split(' ')[1];
    var hm = time.lastIndexOf(':')
    if (hm != -1)
      return time.substring(0, hm);
  }

  return date;
}

/**
 * Start JCCKit applet
 */
function startApplet()
{
  // Graph name
  if (!("name" in getUrlVars()))
  {
    alert("No graph selected");
    return;
  }
  graphName = getUrlVars()["name"];
  loadJSONdata("/config/graphs.json", updateValues);
}
