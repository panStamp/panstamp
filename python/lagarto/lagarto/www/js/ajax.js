/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var dataDoc = null;		// XML document
var jsonDoc = null    // JSON document

/**
 * Load JSON data from server
 *
 * @param filePath: path to the XML file to be loaded
 * @param callback:	callback function to be called after loading the xml file
 */
function loadJSONdata(filePath, callback)
{
  loadXMLdata(filePath, callback, json=true);
}

/**
 * Load XML data from server
 *
 * @param filePath: path to the XML file to be loaded
 * @param callback:	callback function to be called after loading the xml file
 * @param json: true if document has JSON format
 */
function loadXMLdata(filePath, callback, json)
{
  json = typeof json !== 'undefined' ? json : false;

  var dataFile = filePath;

	try	// Internet Explorer
	{
		dataDoc = new ActiveXObject("Microsoft.XMLDOM");
		dataDoc.onreadystatechange = function()
		{
			if (dataDoc.readyState == 4)
				callback();
		}
    if (!json)
  		dataDoc.load(dataFile);
    else
      dataDoc = eval("(" + dataDoc.responseText + ")");
	}
	catch(e)
	{
		try	// Google Chrome, Firefox
		{
			var xmlReq = new window.XMLHttpRequest();

			xmlReq.onreadystatechange = function()
			{
				if (xmlReq.readyState == 4)
				{
					if (xmlReq.status == 200)
					{
            if (!json)
  				    dataDoc = xmlReq.responseXML;
            else
              dataDoc = JSON.parse(xmlReq.responseText);
						callback();
					}
				}
			}
			xmlReq.open("GET", dataFile, true);
  		xmlReq.send(null);
		}
		catch(e)
		{
			try // Opera, etc
			{
			  dataDoc = document.implementation.createDocument("", "", null);
			  dataDoc.onload = callback;
			  dataDoc.load(dataFile);
			}
			catch(e)
			{
				alert("Your browser can not handle this script");
			}
		}
	}
}

/**
 * Return XML document
 */
function getXmlDoc()
{
	return dataDoc;
}

/**
 * Return JSON document
 */
function getJsonDoc()
{
	return dataDoc;
}

