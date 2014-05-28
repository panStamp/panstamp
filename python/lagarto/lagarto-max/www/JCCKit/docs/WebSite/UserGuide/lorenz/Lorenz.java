import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Panel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Properties;

import jcckit.Graphics2DPlotCanvas;
import jcckit.GraphicsPlotCanvas;
import jcckit.data.DataCurve;
import jcckit.data.DataPlot;
import jcckit.data.DataPoint;
import jcckit.util.ConfigParameters;
import jcckit.util.PropertiesBasedConfigData;


/**
 * 
 * 
 * @author Franz-Josef Elmer
 */
public class Lorenz extends Applet {
  private double _b = 8.0 / 3;
  private double _sigma = 10;
  private double _r = 28;
  private double _dt = 0.015;
  private double[] _x = new double[3];
  private double[] _xDot1 = new double[3];
  private double[] _xDot2 = new double[3];
  private double[][] _walk = new double[50][2];
  private int _walkIndex;
  private DataPlot _dataPlot;
  private Thread _animationThread;
  
  public void init() { 
    GraphicsPlotCanvas plotCanvas 
        = createPlotCanvas("true".equals(getParameter("graphics2D")));
    
    _dataPlot = new DataPlot();
    _dataPlot.addElement(new DataCurve("")); // dummy
    reset();
    plotCanvas.connect(_dataPlot);
    
    setLayout(new BorderLayout());
    add(plotCanvas.getGraphicsCanvas(), BorderLayout.CENTER);
    add(createControlPanel(), BorderLayout.SOUTH);
  }

  private GraphicsPlotCanvas createPlotCanvas(boolean graphics2D) {
    Properties props = new Properties();
    ConfigParameters config 
        = new ConfigParameters(new PropertiesBasedConfigData(props));
    props.put("foreground", "0xffffff");
    props.put("background", "0");
    props.put("plot/legendVisible", "false");
    props.put("plot/coordinateSystem/xAxis/minimum", "-20");
    props.put("plot/coordinateSystem/xAxis/maximum", "20");
    props.put("plot/coordinateSystem/xAxis/ticLabelFormat", "%d");
    props.put("plot/coordinateSystem/yAxis/axisLabel", "z");
    props.put("plot/coordinateSystem/yAxis/minimum", "0");
    props.put("plot/coordinateSystem/yAxis/maximum", "50");
    props.put("plot/coordinateSystem/yAxis/ticLabelFormat", "%d");
    props.put("plot/curveFactory/definitions", "curve");
    props.put("plot/curveFactory/curve/initialHintForNextPoint/className",
              "jcckit.plot.ShapeAttributesHint");
    props.put("plot/curveFactory/curve/initialHintForNextPoint/"
              + "initialAttributes/fillColor", "0x50a");
    props.put("plot/curveFactory/curve/initialHintForNextPoint/"
              + "fillColorHSBIncrement", "0.0 0.0 0.018");
    props.put("plot/curveFactory/curve/withLine", "false");
    props.put("plot/curveFactory/curve/symbolFactory/className", 
              "jcckit.plot.CircleSymbolFactory");
    props.put("plot/curveFactory/curve/symbolFactory/size", "0.015");
    
    return graphics2D ? new Graphics2DPlotCanvas(config) 
                      : new GraphicsPlotCanvas(config);
  }
  
  private Panel createControlPanel() {
    Panel controlPanel = new Panel();
    Button startButton = new Button("start");
    startButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                  _animationThread = new Thread() {
                          public void run() {
                            while (_animationThread != null) {
                              try {
                                Thread.sleep(50);
                              } catch (InterruptedException e) {}
                              next();
                            }
                          }
                        };
                  _animationThread.start();
                }
              });
    controlPanel.add(startButton);
    Button stopButton = new Button("stop");
    stopButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                  _animationThread = null;
                }
              });
    controlPanel.add(stopButton);
    Button resetButton = new Button("reset");
    resetButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                  reset();
                }
              });
    controlPanel.add(resetButton);
    
    return controlPanel;
  }
  
  private void reset() {
    _x[0] = 10 * (Math.random() - 0.5);
    _x[1] = 10 * (Math.random() - 0.5);
    _x[2] = 10;
    for (int i = 0; i < _walk.length; i++) {
      _walk[i][0] = _x[0];
      _walk[i][1] = _x[2]; 
    }
    updateCurve();
  }
  
  private void next() {
    integrate();
    _walk[_walkIndex][0] = _x[0];
    _walk[_walkIndex][1] = _x[2];
    _walkIndex = (_walkIndex + 1) % _walk.length;
    updateCurve();
  }

  /** 
   * Integrates differential equations by using a second-order Runge-Kutta
   * method.
   */
  private void integrate() {
    calculateDerivatives(_x, _xDot1);
    for (int i = 0; i < _x.length; i++) {
      _x[i] += _dt * _xDot1[i]; 
    }
    calculateDerivatives(_x, _xDot2);
    for (int i = 0; i < _x.length; i++) {
      _x[i] += 0.5 * _dt * (_xDot2[i] - _xDot1[i]); 
    }
  }
  
  private void calculateDerivatives(double[] x, double[] xDot) {
    xDot[0] = _sigma * (x[1] - x[0]);
    xDot[1] = _r * x[0] - x[1] - x[0] * x[2];
    xDot[2] = x[0] * x[1] - _b * x[2];
  }
  
  private void updateCurve() {
    DataCurve curve = new DataCurve("trajectory");
    for (int i = 0; i < _walk.length; i++) {
      int index = (_walkIndex + i) % _walk.length;
      curve.addElement(new DataPoint(_walk[index][0], _walk[index][1]));
    }
    _dataPlot.replaceElementAt(0, curve);
  }
}
