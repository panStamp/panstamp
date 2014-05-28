/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Fill server list
 */
function fillServers(servers)
{
  var fldServer = document.getElementById("server");

  fldServer.options.length = 0;
  for(var server in servers)
  {
    fldServer.options[fldServer.options.length] = new Option(server, servers[server]);
  }

  onchangeServer();
}

/**
 * Fill endpoints in item1
 */
function fillEndpoints()
{
  var fldEndp = document.getElementById("endp");
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.lagarto;

  fldEndp.options.length = 0;

  swapnet.status.forEach(function(endpoint)
  {
    var endp = endpoint.location + "." + endpoint.name;

    fldEndp.options[fldEndp.options.length] = new Option(endp, endp);
    
  });
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

