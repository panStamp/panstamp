/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update table fields
 */
function updateValues()
{
  var xmlDoc = getXmlDoc();
  var settings = xmlDoc.getElementsByTagName("settings")[0];

  if (settings != null)
  {
    var opt;

    var debug = settings.getElementsByTagName("debug")[0];
    if (debug != null)
      document.getElementById("debug").value = debug.childNodes[0].nodeValue;

    var local = settings.getElementsByTagName("local")[0];
    if (local != null)
      document.getElementById("local").value = local.childNodes[0].nodeValue;

    var remote = settings.getElementsByTagName("remote")[0];
    if (remote != null)
      document.getElementById("remote").value = remote.childNodes[0].nodeValue;

    var update = settings.getElementsByTagName("update")[0];
    if (update != null)
    {
      flag = update.childNodes[0].nodeValue.toLowerCase();
      if (flag == "true" || flag == "yes" || flag == "enabled")
        document.getElementById("update").checked = true;
    }

    var serial = settings.getElementsByTagName("serial")[0];
    if (serial != null)
      document.getElementById("serial").value = serial.childNodes[0].nodeValue;

    var network = settings.getElementsByTagName("network")[0];
    if (network != null)
      document.getElementById("network").value = network.childNodes[0].nodeValue;

    var swapnet = settings.getElementsByTagName("swapnet")[0];
    if (swapnet != null)
      document.getElementById("swapnet").value = swapnet.childNodes[0].nodeValue;
  }
}

