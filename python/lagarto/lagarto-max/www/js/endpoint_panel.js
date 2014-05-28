/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var servers;

/**
 * Create values
 */
function createValues()
{
  var jsonDoc = getJsonDoc();
  servers = jsonDoc.http_servers;
  for(var server in servers)
    loadJSONdata("/command/get_endpoint_list/?server=" + servers[server], addNetwork);
}

/**
 * Add network
 */
function addNetwork()
{
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.lagarto;

  // Process name
  procnameParag = document.createElement('p');
  procnameParag.setAttribute("class", "netlabel");
  procnameParag.setAttribute("align", "center");
  procname = document.createTextNode("Network: " + swapnet.procname);
  proclink = document.createElement("a");
  proclink.setAttribute("href", "http://" + servers[swapnet.procname]);
  proclink.appendChild(procname);
  procnameParag.appendChild(proclink);

  // Endpoint table
  tableNet = document.createElement('table');
  tableNet.setAttribute("id", swapnet.procname);
  tableNet.setAttribute("width", "550px");
  tableNet.setAttribute("border", "0");
  tableNet.setAttribute("cellspacing", "0");
  tableNet.setAttribute("cellpadding", "0");

  row = tableNet.insertRow(0);
  // ID
  cell = row.insertCell(0);
  cell.className = "cellhead";
  label = document.createTextNode("ID");
  cell.appendChild(label);
  cell.style.nowrap="nowrap";
  cell.style.width="10%"; 
  row.appendChild(cell);
  // Location
  cell = row.insertCell(0);
  cell.className = "cellhead";
  label = document.createTextNode("Location");
  cell.appendChild(label);
  cell.style.nowrap="nowrap";
  cell.style.width="25%"; 
  row.appendChild(cell);
  // Name
  cell = row.insertCell(0);
  cell.className = "cellhead";
  label = document.createTextNode("Name");
  cell.appendChild(label);
  cell.style.nowrap="nowrap";
  cell.style.width="25%"; 
  row.appendChild(cell);
  // Value
  cell = row.insertCell(0);
  cell.className = "cellhead";
  label = document.createTextNode("Value");
  cell.appendChild(label);
  cell.style.nowrap="nowrap";
  cell.style.width="15%"; 
  row.appendChild(cell);
  // Command
  cell = row.insertCell(0);
  cell.className = "cellhead";
  label = document.createTextNode("Command");
  cell.appendChild(label);
  cell.style.nowrap="nowrap";
  cell.style.width="25%"; 
  row.appendChild(cell);

  tabdiv = document.getElementById("divcontainer");
  tabdiv.appendChild(procnameParag);
  tabdiv.appendChild(tableNet);

  swapnet.status.forEach(addEndpoint, tableNet);
}

/**
 * Add endpoint
 */
function addEndpoint(endpoint)
{
  var row, cell, label, command, val;
  var procName = this.id;
  var httpAddr = servers[procName];
  row = this.insertRow(this.rows.length);

  // Link to config page
  cell = row.insertCell(0);
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "http://" + httpAddr + "/config_endpoint.html/?id=" + endpoint.id);
  cell.appendChild(cfglink);
  // Endpoint ID
  cell.className = "cellitem";
  label = document.createTextNode(endpoint.id);
  label.value = document.createTextNode(endpoint.id);
  cfglink.appendChild(label);
  // Location
  cell = row.insertCell(1);
  cell.className = "cellitem";
  label = document.createTextNode(endpoint.location);
  cell.appendChild(label);
  // Name
  cell = row.insertCell(2);
  cell.className = "cellitem";
  label = document.createTextNode(endpoint.name);
  cell.appendChild(label);
  // Value
  cell = row.insertCell(3);
  cell.className = "cellitem";
  val = document.createElement("input");
  val.type = "text";
  val.className = "inputnoedit1";
  val.readOnly = "readOnly";
  val.id = this.id + "." + endpoint.id;
  val.value = endpoint.value
  if ("unit" in endpoint)
  val.value += " " + endpoint.unit 
  cell.appendChild(val);
  // Command
  cell = row.insertCell(4);
  cell.className = "cellitem";

  if (endpoint.direction == "out")
  {
    if (endpoint.type == "bin")
    {
      // ON button
  		command = document.createElement("input");
      command.type = "button";
      command.className = "button_b";
      command.value = "on";
      var reqOn = "/command/set_endpoint_value/?procname=" + procName + "&id=" + endpoint.id + "&value=ON"
      command.onclick = function() {loadJSONdata(reqOn, updateValues);}
      cell.appendChild(command);
      // OFF button
  		command = document.createElement("input");
      command.type = "button";
      command.className = "button_b";
      command.value = "off";
      var reqOff = "/command/set_endpoint_value/?procname=" + procName + "&id=" + endpoint.id + "&value=OFF"
      command.onclick = function() {loadJSONdata(reqOff, updateValues);}
      cell.appendChild(command);
    }
    else
    {
      // "Set" text input
      val = document.createElement("input");
      val.type = "text";
      val.className = "inputdatamini";
      val.id = "cv" + endpoint.id;
      cell.appendChild(val);

      // "Set" button
      command = document.createElement("input");
      command.type = "submit";
      command.value = "set"
      command.className = "button_b";
      command.onclick = function() {loadJSONdata("/command/set_endpoint_value/?procname=" + procName + "&id=" + endpoint.id + "&value=" + document.getElementById("cv" + endpoint.id).value, updateValues);}
      cell.appendChild(command);
    }
  }
}

/**
 * Update network data
 */
function updateNetworks()
{
  var jsonDoc = getJsonDoc();
  servers = jsonDoc.http_servers;
  for(var server in servers)
    loadJSONdata("/command/get_endpoint_list/?server=" + servers[server], updateValues);
}

/**
 * Update values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.lagarto;

  var tableNet = document.getElementById(swapnet.procname);

  swapnet.status.forEach(function(endpoint)
  {
    valField = document.getElementById(swapnet.procname + "." + endpoint.id);
    if (valField != null)
    {
      valField.value = endpoint.value;
      if ("unit" in endpoint)
        valField.value += " " + endpoint.unit
    }
  });
}

