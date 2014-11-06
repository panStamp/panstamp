/* Copyright (c) panStamp SLU 2014 */

// Global array of endpoints
var arrEndpoints = new Array();
// Our transmission already exists
var existingTx = false;
// Endpoints were edited
var endpointsEdited = false;

/**
 * Server data received
 */
function updateServers()
{
  var jsonDoc = getJsonDoc();
  servers = jsonDoc.http_servers;

  fillServers(servers);
}

/**
 * Update table values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var cloud = jsonDoc.cloud;
   
  var txName = "";
  if ("name" in getUrlVars())
  {
    txName = decodeURIComponent(getUrlVars()["name"]);
    existingTx = true;

    if (txName in cloud)
    {
      var transmission = cloud[txName];
      document.getElementById("txname").value = txName;
      document.getElementById("interval").value = transmission.interval;
      document.getElementById("apikey").value = transmission.apikey;

      // Fill endpoint table
      for (fieldId in transmission.endpoints)
      {
        var endpoint = transmission.endpoints[fieldId];
        
        var dot = endpoint.indexOf('.');
        if (dot == -1)
        {
          alert("Endpoint " + endpoint + " has incorrect format");
          return;
        }

        var server = endpoint.substring(0, dot);
        var endp = endpoint.substring(dot+1);
        addEndpoint(server, endp, fieldId);
      }
    }
  }
}

/**
 * Add endpoint to the table
 */
function addEndpointFromMenu()
{
  var serverField = document.getElementById("server");
  var index = serverField.selectedIndex;
  var server = serverField.options[index].text;
  var endpoint = document.getElementById("endp").value;
  var fieldId = document.getElementById("fieldid").value;

  addEndpoint(server, endpoint, fieldId);

  if (!endpointsEdited)
    endpointsEdited = true;
}

/**
 * Add endpoint to the table
 */
function addEndpoint(server, endpoint, fieldId)
{
  var endpData = server + "." + endpoint;

  if (arrEndpoints.indexOf(endpData) > -1)
  {
    alert(endpData + " already included in table");
    return;
  }

  var table = document.getElementById("endptable");
  var row = table.insertRow(table.rows.length);

  // Endpoint data
  cell = row.insertCell(0);
  cell.className = "cellitem";
  var data = document.createTextNode(endpData);
  cell.appendChild(data);

  // ID
  cell = row.insertCell(1);
  cell.className = "cellitem";
  var fieldid = document.createTextNode(fieldId);
  cell.appendChild(fieldid);


  // Save endpoint data in global array
  arrEndpoints[fieldId] = endpData;
}

/**
 * Clear endpoint table
 */
function clearTable()
{
  if (!endpointsEdited)
    endpointsEdited = true;

  var table = document.getElementById("endptable");

  // Clear table
  while (table.rows.length > 1)
    table.deleteRow(1);

  // Clear global array of endpoints
  arrEndpoints = [];
}

/**
 * Save form data
 */
function save()
{
  var count = 0;
  var result = "{";
 
  for(item in arrEndpoints)
  {
    if (count > 0)
      result += ",";
    result += "\"" + item + "\"" + ":" + "\"" + arrEndpoints[item] + "\"";
    count++;
  }
  result += "}";

  if (count == 0)
  {
    alert("Tables need at least one endpoint");
    return;
  }
  else if (existingTx && endpointsEdited)
  {
    if (!confirm("Saving data will clear the table contents.\nDo you want to continue?"))
      return;
  }
   
  document.getElementById("endpoints").value = result;

  document.getElementById("table_data_form").submit();
}
