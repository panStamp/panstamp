/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update table fields
 */
function updateValues()
{
  var xmlDoc = getXmlDoc();
  var section = xmlDoc.getElementsByTagName("serial")[0];

  if (section != null)
  {
    var opt;

    var port = section.getElementsByTagName("port")[0];
    if (port != null)
      document.getElementById("port").value = port.childNodes[0].nodeValue;

    var speed = section.getElementsByTagName("speed")[0];
    if (speed != null)
      document.getElementById("speed").value = speed.childNodes[0].nodeValue;
  }
}

