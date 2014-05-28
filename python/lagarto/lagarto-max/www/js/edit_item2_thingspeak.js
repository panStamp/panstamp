/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var codeLine, statement;

/**
 * Update values
 */
function updateValues()
{
  codeLine = window.parent.codeLine;
  statement = window.parent.statement;

  var jsonDoc = getJsonDoc();
  var servers = jsonDoc.http_servers;
  fillServers(servers);

  var fldApiKey = document.getElementById("apikey");
  var fldFieldId = document.getElementById("fieldid");

  var end, start = codeLine.indexOf(", \"");
  if (start > -1)
  {
    start += 3;
    end = codeLine.indexOf("\", \"", start);
    if (end > -1)
    {
      fldApiKey.value = codeLine.substring(start, end);
      start = end + 4;
      end = codeLine.indexOf("\")", start);
      if (end > -1)
        fldFieldId.value = codeLine.substring(start, end);
    }
  }
}

/**
 * Fill server list
 */
function fillServers(servers)
{
  var dot = statement[3].indexOf(".");
  var currVal = statement[3].substring(0, dot);
  var currValFound = false;
  var fldServer = document.getElementById("server");

  if (currVal == "")
    currValFound = true;

  fldServer.options.length = 0;
  for(var server in servers)
  {
    fldServer.options[fldServer.options.length] = new Option(server, servers[server]);

    if (!currValFound)
    {
      if (server == currVal)
      {
        fldServer.options[fldServer.options.length-1].selected = true;
        currValFound = true;
      }
    }
  }
  if (!currValFound)
  {
    fldServer.options[fldServer.options.length] = new Option(currVal, "");
    fldServer.options[fldServer.options.length-1].selected = true;

    var fldEndp = document.getElementById("endp");
    var endp = statement[3].substring(dot+1);
    fldEndp.options.length = 0;
    fldEndp.options[fldEndp.options.length] = new Option(endp, endp);
  }

  onchangeServer();
}

/**
 * Lagarto server selected
 */
function onchangeServer()
{
  var server = document.getElementById("server").value;

  if (server != "")
    loadJSONdata("/command/get_endpoint_list/?server=" + server, fillEndpoints);
}

/**
 * Fill endpoints in item2
 */
function fillEndpoints()
{
  var dot = statement[3].indexOf(".");
  var currVal = statement[3].substring(dot+1);
  var currValFound = false;
  var fldEndp = document.getElementById("endp");
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.lagarto;

  if (currVal.indexOf('.') == -1)
    currValFound = true;

  fldEndp.options.length = 0;

  endpointTypes = {};
  swapnet.status.forEach(function(endpoint)
  {
    var endp = endpoint.location + "." + endpoint.name;

    if (!currValFound)
    {
      if (endp == currVal)
        currValFound = true;
    }
    fldEndp.options[fldEndp.options.length] = new Option(endp, endp);
    
    endpointTypes[endp] = endpoint.type;
  });

  if (!currValFound)
  {
    fldEndp.options[fldEndp.options.length] = new Option(currVal, currVal);
    endpointTypes[currVal] = "num";
  }

  fldEndp.value = currVal;
}

/**
 * Return python representation of item2
 */
function getItem2()
{
  var i = document.getElementById("server").selectedIndex;
  if (i == -1)
    return null;
  var server = document.getElementById("server").options[i].text;
  var endp = document.getElementById("endp").value;
  var apiKey = document.getElementById("apikey").value;

  var fieldId = document.getElementById("fieldid").value;

  var item2 = "\"" + server + "." + endp + "\", \"" + apiKey + "\", \"" + fieldId + "\"";

  return item2;
}

