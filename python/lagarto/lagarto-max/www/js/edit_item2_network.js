/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var item2;

/**
 * Update values
 */
function updateValues()
{
  item2 = window.parent.statement[3];
  var fldServer = document.getElementById("server");
  var fldEndp = document.getElementById("endp");
  var fldModif = document.getElementById("modif");

  var dot = item2.indexOf(".");
  var currVal = item2.substring(0, dot);
  var currValFound = false;
  var servers = window.parent.getServers();

  if (currVal == "")
    currValFound = true;

  fldServer.options.length = 0;
  for(var i=0 ; i < servers.length ; i++)
  {
    fldServer.options[i] = new Option(servers[i].text, servers[i].value);

    if (!currValFound)
    {
      if (servers[i].text == currVal)
      {
        fldServer.options[i].selected = true;
        currValFound = true;
      }
    }
  }
  if (!currValFound)
  {
    fldServer.options[fldServer.options.length] = new Option(currVal, "");
    fldServer.options[fldServer.options.length-1].selected = true;

    var fldEndp = document.getElementById("endp");
    var endp = item2.substring(dot+1);
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
  var space = item2.indexOf(" ");
  var dot = item2.indexOf(".");
  var endp = item2.substring(0, space);
  var currVal = item2.substring(dot+1);
  var currValFound = false;
  var fldEndp = document.getElementById("endp");
  var fldModif = document.getElementById("modif");
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.lagarto;

  if (currVal.indexOf('.') == -1)
    currValFound = true;

  fldEndp.options.length = 0;

  swapnet.status.forEach(function(endpoint)
  {
    var endp = endpoint.location + "." + endpoint.name;

    if (!currValFound)
    {
      if (endp == currVal)
        currValFound = true;
    }
    fldEndp.options[fldEndp.options.length] = new Option(endp, endp);
  });

  if (!currValFound)
    fldEndp.options[fldEndp.options.length] = new Option(currVal, currVal);

  fldEndp.value = currVal;

  if (space > -1)
    fldModif.value = item2.substring(space+1);
}

/**
 *  Switch item2 to network type
 */
function switchItem2()
{
  if (!document.getElementById("netopt").checked)
  {
    var endpType = window.parent.getEndpointType();

    if (endpType == "bin")
	    window.parent.document.getElementById("item2box").src = "../edit_item2_state.html";
    else
      window.parent.document.getElementById("item2box").src = "../edit_item2_val.html";
  }
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
  var modif = document.getElementById("modif").value;

  var item2 = "\"" + server + "." + endp + "\"";

  if (window.parent.statementType == "condition")
    item2 = "network.get_value(" + item2 + ")";

  if (modif != "")
    item2 += " " + modif;

  return item2;
}

