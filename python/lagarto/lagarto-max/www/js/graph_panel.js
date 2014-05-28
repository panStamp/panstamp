/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Create values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var graphs = jsonDoc.graphs;

  for(var graphName in graphs)
    addGraph(graphName, graphs[graphName]);
}

/**
 * Add graph to the table
 */
function addGraph(graphName, graph)
{
  var graphTable = document.getElementById("graphtable");
  var row = graphTable.insertRow(graphTable.rows.length);

  // Link to the graph page
  cell = row.insertCell(0);
  graphlink = document.createElement("a");
  graphlink.setAttribute("href", "/show_graph.html/?name=" + graphName);
  cell.appendChild(graphlink);
  // Graph name
  cell.className = "cellitem";
  tName = document.createTextNode(graphName);
  graphlink.appendChild(tName);

  // Title
  cell = row.insertCell(1);
  cell.className = "cellitem";
  tableInterval = document.createTextNode(graph.title);
  cell.appendChild(tableInterval);

  // Actions
  cell = row.insertCell(2);
  cell.className = "cellaction";
  // Delete graph
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/command/delete_graph/?name=" + graphName);
  cfglink.onclick = function() {return confirm("Delete graph?");};
  cfglink.setAttribute("alt", "delete");
  cfglink.style.pointer = "cursor";
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/delete.png");
  img.title = "delete";
  cfglink.appendChild(img);
  // Edit graph
  cell.className = "cellaction";
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/edit_graph.html/?name=" + graphName);
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/edit.png");
  img.title = "edit";
  cfglink.appendChild(img);
}


