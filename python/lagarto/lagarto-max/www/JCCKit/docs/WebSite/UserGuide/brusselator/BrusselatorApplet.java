/*
 * Copyright 2003, Franz-Josef Elmer, All rights reserved
 */
import jcckit.util.ConfigParameters;
import jcckit.util.AppletBasedConfigData;

import java.applet.*;
import java.awt.*;
import java.util.*;

public class BrusselatorApplet extends Applet {
  private ConfigParameters _config
      = new ConfigParameters(new AppletBasedConfigData(this));

  public void init() {
    setBackground(_config.getColor("background", getBackground()));
    Brusselator brusselator = createBrusselator();
    BrusselatorPlot plot = new BrusselatorPlot(_config, brusselator);

    setLayout(new BorderLayout());
    Panel p = new Panel();
    p.setLayout(new FlowLayout(FlowLayout.LEFT));
    p.add(new TimeView(brusselator));
    add(p, BorderLayout.NORTH);
    add(plot.getGraphicsCanvas(), BorderLayout.CENTER);
    add(new BrusselatorController(brusselator).getControlPanel(), 
        BorderLayout.SOUTH);
  }

  private Brusselator createBrusselator() {
    double dx = _config.getDouble("dx", 1);
    double dt = _config.getDouble("dt", 0.05);
    double size = _config.getDouble("L", 100);
    Brusselator brusselator = new Brusselator(dx, dt, size);
    brusselator.setNoiseFactor(_config.getDouble("noiseFactor", 1e-4));
    brusselator.setAlpha(_config.getDouble("alpha", 1.8));
    brusselator.setBeta(_config.getDouble("beta", 1));
    brusselator.setD(_config.getDouble("d", 9));
    brusselator.reset();
    brusselator.addNoise();
    double[] indices = _config.getDoubleArray("indices", new double[0]);
    double[] u = _config.getDoubleArray("u", new double[0]);
    double[] v = _config.getDoubleArray("v", new double[0]);
    int len = Math.min(Math.min(u.length, v.length), indices.length);
    for (int i = 0; i < len; i++) {
      int index = (int) (indices[i] + 0.5);
      brusselator.setU(index, u[i]);
      brusselator.setV(index, v[i]);
    }
    return brusselator;
  }
}
