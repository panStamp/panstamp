/*
 * Copyright 2003, Franz-Josef Elmer, All rights reserved
 */
import jcckit.GraphicsPlotCanvas;
import jcckit.data.DataPlot;
import jcckit.data.DataCurve;
import jcckit.data.DataPoint;
import jcckit.util.ConfigParameters;

import java.awt.event.*;

public class BrusselatorPlot extends GraphicsPlotCanvas {
  private Brusselator _brusselator;

  public BrusselatorPlot(ConfigParameters config, Brusselator brusselator) {
    super(config);
    _brusselator = brusselator;
    brusselator.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent event) {
            replot();
          }
        });
    replot();
  }

  private void replot() {
    DataPlot plot = new DataPlot();
    DataCurve uCurve = new DataCurve("u");
    DataCurve vCurve = new DataCurve("v");
    double dx = _brusselator.getDx();
    for (int i = 0, n = _brusselator.getNumberOfPoints(); i < n; i++) {
      uCurve.addElement(new DataPoint(dx * i, _brusselator.getU(i)));
      vCurve.addElement(new DataPoint(dx * i, _brusselator.getV(i)));
    }
    plot.addElement(uCurve);
    plot.addElement(vCurve);
    connect(plot);
  }
}
