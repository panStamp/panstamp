import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Frame;
import java.awt.Panel;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Properties;

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
public class AnimatedChart extends Applet {
  private double[] _data = new double[]
          {55.5, 34.2, 47.4, 53.1, 69.9, 68.7, 81.1};
  private DataPlot _dataPlot;

  public void init() {
    GraphicsPlotCanvas plotCanvas = createPlotCanvas();

    _dataPlot = new DataPlot();
    _dataPlot.addElement(new DataCurve(""));
    plotCanvas.connect(_dataPlot);

    setLayout(new BorderLayout());
    add(plotCanvas.getGraphicsCanvas(), BorderLayout.CENTER);
    add(createControlPanel(), BorderLayout.SOUTH);
  }

  private GraphicsPlotCanvas createPlotCanvas() {
    Properties props = new Properties();
    ConfigParameters config
        = new ConfigParameters(new PropertiesBasedConfigData(props));
    props.put("plot/legendVisible", "false");
    props.put("plot/coordinateSystem/xAxis/minimum", "-0.5");
    props.put("plot/coordinateSystem/xAxis/maximum", "6.5");
    props.put("plot/coordinateSystem/xAxis/axisLabel", "");
    props.put("plot/coordinateSystem/xAxis/ticLabelFormat/className",
              "jcckit.plot.TicLabelMap");
    props.put("plot/coordinateSystem/xAxis/ticLabelFormat/map",
              "0=Mo;1=Tu;2=We;3=Th;4=Fr;5=Sa;6=Su");
    props.put("plot/coordinateSystem/yAxis/axisLabel", "growth rate");
    props.put("plot/coordinateSystem/yAxis/maximum", "100");
    props.put("plot/coordinateSystem/yAxis/ticLabelFormat", "%d%%");
    props.put("plot/curveFactory/definitions", "curve");
    props.put("plot/curveFactory/curve/withLine", "false");
    props.put("plot/curveFactory/curve/symbolFactory/className", 
              "jcckit.plot.BarFactory");
    props.put("plot/curveFactory/curve/symbolFactory/attributes/className", 
              "jcckit.graphic.ShapeAttributes");
    props.put("plot/curveFactory/curve/symbolFactory/attributes/fillColor", 
              "0xfe8000");
    props.put("plot/curveFactory/curve/symbolFactory/attributes/lineColor", 
              "0");
    props.put("plot/curveFactory/curve/symbolFactory/size", "0.08");
    props.put("plot/initialHintForNextCurve/className", 
              "jcckit.plot.PositionHint");
    props.put("plot/initialHintForNextCurve/position", "0 0.1");

    return new GraphicsPlotCanvas(config);
  }
  
  private Panel createControlPanel() {
    Panel controlPanel = new Panel();
    Button startButton = new Button("animate");
    startButton.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                  new Thread() {
                          public void run() {
                            animate();
                          }
                        }.start();
                }
              });
    controlPanel.add(startButton);
    
    return controlPanel;
  }
  
  private void animate() {
    DataCurve curve = new DataCurve("");
    for (int i = 0; i < _data.length; i++) {
      curve.addElement(new DataPoint(i, 0));
    }
    _dataPlot.replaceElementAt(0, curve);

    for (int i = 0; i < _data.length; i++) {
      double x = i;
      double y = 0;
      while (y < _data[i]) {
        try {
          Thread.sleep(50);
        } catch (InterruptedException e) {}
        y = Math.min(_data[i], y + 5);
        curve.replaceElementAt(i, new DataPoint(x, y));
      }
    }
  }

  
  public static void main(String[] args) {
    Frame frame = new Frame("Animated Chart");
    frame.addWindowListener(new WindowAdapter() {
              public void windowClosing(WindowEvent event) {
                System.exit(0);
              } 
            });
    Applet applet = new AnimatedChart();
    applet.init();
    frame.add(applet);
    frame.setSize(600, 500);
    frame.show();
  }
}
