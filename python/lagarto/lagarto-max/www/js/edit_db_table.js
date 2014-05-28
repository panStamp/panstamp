/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

// Global array of endpoints
var arrEndpoints = new Array();
// Our table already exists
var existingTable = false;
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
  var database = jsonDoc.database;

  var tableName = "";
  if ("name" in getUrlVars())
  {
    tableName = getUrlVars()["name"];
    existingTable = true;

    if (tableName in database)
    {
      var table = database[tableName];
      document.getElementById("tablename").value = tableName;
      document.getElementById("interval").value = table.interval;

      // Fill endpoint table
      table.endpoints.forEach(function(endpoint)
      {
        var dot = endpoint.indexOf('.');
        if (dot == -1)
        {
          alert("Endpoint " + endpoint + " has incorrect format");
          return;
        }

        var server = endpoint.substring(0, dot);
        var locname = endpoint.substring(dot+1);
        addEndpoint(server, locname);
      });
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

  addEndpoint(server, endpoint);

  if (!endpointsEdited)
    endpointsEdited = true;
}

/**
 * Add endpoint to the table
 */
function addEndpoint(server, endpoint)
{
  var dot = endpoint.lastIndexOf('.');
  if (dot == -1)
    return;
  var colName = endpoint.substring(dot+1);

  var endpData = server + "." + endpoint;

  if (arrEndpoints.indexOf(endpData) > -1)
  {
    alert(endpData + " already included in table");
    return;
  }
  else
  {
    for(var i=0 in arrEndpoints)
    {
      dot = arrEndpoints[i].lastIndexOf('.');
      if (arrEndpoints[i].substring(dot+1) == colName)
      {
        alert("Column " + colName + " already exists in table.\nPlease rename the endpoint before associating it to a table");
        return;
      }
    }
  }

  var table = document.getElementById("endptable");
  var row = table.insertRow(table.rows.length);

  // Endpoint data
  cell = row.insertCell(0);
  cell.className = "cellitem";
  var data = document.createTextNode(endpData);
  cell.appendChild(data);

  // Column name
  cell = row.insertCell(1);
  cell.className = "cellitem";
  var colname = document.createTextNode(colName);
  cell.appendChild(colname);


  // Save endpoint data in global array
  arrEndpoints.push(endpData);
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
  if (arrEndpoints.length == 0)
  {
    alert("Tables need at least one endpoint");
    return;
  }

  if (existingTable && endpointsEdited)
  {
    if (!confirm("Saving data will clear the table contents.\nDo you want to continue?"))
      return;
  }

  document.getElementById("endpoints").value = arrEndpoints;

  document.getElementById("table_data_form").submit();
}
