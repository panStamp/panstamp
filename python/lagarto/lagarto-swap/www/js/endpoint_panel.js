/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Create values
 */
function createValues()
{
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.lagarto;
  swapnet.status.forEach(addEndpoint);
}

/**
 * Add endpoint
 */
function addEndpoint(endpoint)
{
  var nettable = document.getElementById("nettable");
  var row, cell, label, command, val;

  row = nettable.insertRow(nettable.rows.length);

  // Link to config page
  cell = row.insertCell(0);
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "config_endpoint.html/?id=" + endpoint.id);
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
  val.id = endpoint.id;
  val.value = endpoint.value
  if ("unit" in endpoint)
  val.value += " " + endpoint.unit 
  cell.appendChild(val);
  // Time stamp
  val = document.createElement("input");
  val.type = "hidden";
  val.id = "ts_" + endpoint.id;
  val.value = endpoint.timestamp
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
      var reqOn = "values/?id=" + endpoint.id + "&value=ON"
      command.onclick = function() {loadJSONdata(reqOn, updateValues);}
      cell.appendChild(command);
      // OFF button
  		command = document.createElement("input");
      command.type = "button";
      command.className = "button_b";
      command.value = "off";
      var reqOff = "values/?id=" + endpoint.id + "&value=OFF"
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
      command.onclick = function() {loadJSONdata("values/?id=" + endpoint.id + "&value=" + document.getElementById("cv" + endpoint.id).value, updateValues);}
      cell.appendChild(command);
    }
  }
}

/**
 * Update values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.lagarto;
 
  swapnet.status.forEach(function(endpoint)
  {
    valField = document.getElementById(endpoint.id);
    if (valField != null)
    {
      valField.value = endpoint.value;
      if ("unit" in endpoint)
        valField.value += " " + endpoint.unit
    }
  });
}

