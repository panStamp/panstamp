from api import TimeAPI as clock, NetworkAPI as network, CloudAPI as cloud

class WebScripts:
    @staticmethod
    def evn_00000001():
        """
        Upload temperature2 to GS
        """
        if False:
            pass
        elif network.event[0] == "Lagarto-SWAP.SWAP.Temperature2":
            pass
        else:
            return
        cloud.push_grovestreams("Lagarto-SWAP.SWAP.Temperature2", "eca3d79e-7b6f-3b69-ab26-64c7d8575696", "")

    @staticmethod
    def evn_00000002():
        """
        Upload voltage2 to GS
        """
        if False:
            pass
        elif network.event[0] == "Lagarto-SWAP.SWAP.Voltage2":
            pass
        else:
            return
        cloud.push_grovestreams("Lagarto-SWAP.SWAP.Voltage2", "eca3d79e-7b6f-3b69-ab26-64c7d8575696", "tmpVoltageId")

    @staticmethod
    def evn_00000003():
        """
        Upload temperature1 to GS
        """
        if False:
            pass
        elif network.event[0] == "Lagarto-SWAP.SWAP.Temperature1":
            pass
        else:
            return
        cloud.push_grovestreams("Lagarto-SWAP.SWAP.Temperature1", "eca3d79e-7b6f-3b69-ab26-64c7d8575696", "tmpTemperatureId")

    @staticmethod
    def evn_00000004():
        """
        Upload humidity1 to GS
        """
        if False:
            pass
        elif network.event[0] == "Lagarto-SWAP.SWAP.Humidity1":
            pass
        else:
            return
        cloud.push_grovestreams("Lagarto-SWAP.SWAP.Humidity1", "eca3d79e-7b6f-3b69-ab26-64c7d8575696", "tmpHumidityId")

    @staticmethod
    def evn_00000005():
        """
        Upload voltage1 to GS
        """
        if False:
            pass
        elif network.event[0] == "Lagarto-SWAP.SWAP.Voltage1":
            pass
        else:
            return
        cloud.push_grovestreams("Lagarto-SWAP.SWAP.Voltage1", "eca3d79e-7b6f-3b69-ab26-64c7d8575696", "tmpVoltageId")

    @staticmethod
    def evn_00000006():
        """
        Upload voltage3 to GS
        """
        if False:
            pass
        elif network.event[0] == "Lagarto-SWAP.SWAP.Voltage3":
            pass
        else:
            return
        cloud.push_grovestreams("Lagarto-SWAP.SWAP.Voltage3", "eca3d79e-7b6f-3b69-ab26-64c7d8575696", "tmpVoltageId")

    @staticmethod
    def evn_00000007():
        """
        Upload temperature3 to GS
        """
        if False:
            pass
        elif network.event[0] == "Lagarto-SWAP.SWAP.Temperature3":
            pass
        else:
            return
        cloud.push_grovestreams("Lagarto-SWAP.SWAP.Temperature3", "eca3d79e-7b6f-3b69-ab26-64c7d8575696", "tmpTemperatureId")
