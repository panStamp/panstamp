import java.applet.Applet;
import java.awt.BorderLayout;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;
import java.awt.event.MouseMotionListener;
import java.util.Properties;

import jcckit.GraphicsPlotCanvas;
import jcckit.data.DataCurve;
import jcckit.data.DataPlot;
import jcckit.data.DataPoint;
import jcckit.graphic.GraphPoint;
import jcckit.graphic.GraphicalElement;
import jcckit.graphic.Rectangle;
import jcckit.graphic.ShapeAttributes;
import jcckit.plot.CartesianCoordinateSystem;
import jcckit.util.ConfigParameters;
import jcckit.util.PropertiesBasedConfigData;

/**
 * Example applet demonstrating zooming interaction.
 * The applet shows a snowflake curve (Koch curve). The user can define
 * a zoom window by dragging the mouse. A double-click goes back to the
 * original size. 
 * 
 * @author Franz-Josef Elmer
 */
public class ZoomExample extends Applet 
{
  private static double X_MIN_DEFAULT = -1.1;
  private static double X_MAX_DEFAULT = 1.1;
  private static double Y_MIN_DEFAULT = -1.1;
  private static double Y_MAX_DEFAULT = 1.1;
  
  private static final int NUMBER_OF_ITERATIONS = 6;
  private static final double H = 1 / (2 * Math.sqrt(3));
  
  private static final ShapeAttributes MARKER_ATTRIBUTES
                          = new ShapeAttributes(null, Color.gray, 0, null);

  private GraphicsPlotCanvas _plotCanvas;
  private GraphPoint _anchor;

  private MouseListener _mouseListener = new MouseAdapter()
      {
        public void mouseClicked(MouseEvent event)
        {
          if (event.getClickCount() > 1)
          {
            resetViewWindow();
          }
        }
    
        public void mousePressed(MouseEvent event)
        {
          _anchor = getPosition(event);
          drawMarker(_anchor);
        }
        
        public void mouseReleased(MouseEvent event)
        {
          changeViewingWindow(getPosition(event));
        }
      };

  private MouseMotionListener _mouseMotionListener = new MouseMotionAdapter()
      {
        public void mouseDragged(MouseEvent event)
        {
          drawMarker(getPosition(event));
        }
      };
  
  public void init() { 
    createPlotCanvas();
    _plotCanvas.connect(createData());
    
    setLayout(new BorderLayout());
    Canvas graphicsCanvas = _plotCanvas.getGraphicsCanvas();
    graphicsCanvas.addMouseListener(_mouseListener);
    graphicsCanvas.addMouseMotionListener(_mouseMotionListener);
    add(graphicsCanvas, BorderLayout.CENTER);

    resetViewWindow();
}
  
  private void createPlotCanvas() {
    Properties props = new Properties();
    props.put("background", "0xffffff");
    props.put("paper", "0 0 1 1");
    props.put("plot/legendVisible", "false");
    _plotCanvas = new GraphicsPlotCanvas(
                    new ConfigParameters(new PropertiesBasedConfigData(props)));
  }
  
  private void resetViewWindow()
  {
    setCoordinateSystem(X_MIN_DEFAULT, Y_MIN_DEFAULT, 
                        X_MAX_DEFAULT, Y_MAX_DEFAULT);
  }
  
  private void setCoordinateSystem(double xMin, double yMin, 
                                   double xMax, double yMax)
  {
    Properties props = new Properties();
    props.put("xAxis/minimum", Double.toString(xMin));
    props.put("xAxis/maximum", Double.toString(xMax));
    props.put("xAxis/ticLabelFormat", "%1.3f");
    props.put("xAxis/axisLabel", "");
    props.put("yAxis/minimum", Double.toString(yMin));
    props.put("yAxis/maximum", Double.toString(yMax));
    props.put("yAxis/ticLabelFormat", "%1.3f");
    props.put("yAxis/axisLabel", "");
    props.put("yAxis/axisLength", "0.8");
    CartesianCoordinateSystem cs = new CartesianCoordinateSystem(
                  new ConfigParameters(new PropertiesBasedConfigData(props)));
    _plotCanvas.getPlot().setCoordinateSystem(cs);
  }

  private DataPlot createData()
  {
    DataPlot dataPlot = new DataPlot();
    DataCurve dataCurve = new DataCurve("");
    DataPoint a = new DataPoint(-3 * H, 0.5);
    DataPoint b = new DataPoint(3 * H, 0.5);
    DataPoint c = new DataPoint(0, -1);
    makeKochCurve(dataCurve, a, b, NUMBER_OF_ITERATIONS);
    makeKochCurve(dataCurve, b, c, NUMBER_OF_ITERATIONS);
    makeKochCurve(dataCurve, c, a, NUMBER_OF_ITERATIONS);
    dataPlot.addElement(dataCurve);
    return dataPlot;
  }
  
  private void makeKochCurve(DataCurve curve, DataPoint p1, DataPoint p2, 
                             int numberOfIterations)
  {
    if (numberOfIterations == 0)
    {
      curve.addElement(p1);
      curve.addElement(p2);
    } else 
    {
      numberOfIterations--;
      double x1 = p1.getX();
      double y1 = p1.getY();
      double dx = p2.getX() - x1;
      double dy = p2.getY() - y1;
      DataPoint a = new DataPoint(x1 + dx / 3, y1 + dy / 3);
      DataPoint b = new DataPoint(x1 + dx / 2 - H * dy, y1 + dy / 2 + H * dx);
      DataPoint c = new DataPoint(x1 + 2 * dx / 3, y1 + 2 * dy / 3);
      makeKochCurve(curve, p1, a, numberOfIterations);
      makeKochCurve(curve, a, b, numberOfIterations);
      makeKochCurve(curve, b, c, numberOfIterations);
      makeKochCurve(curve, c, p2, numberOfIterations);
    }
  }

  private GraphPoint getPosition(MouseEvent event)
  {
    return _plotCanvas.mapCursorPosition(event.getX(), event.getY());
  }

  private void drawMarker(GraphPoint point)
  {
    double x0 = _anchor.getX();
    double y0 = _anchor.getY();
    double x1 = point.getX();
    double y1 = point.getY();
    GraphPoint center = new GraphPoint(0.5 * (x0 + x1), 0.5 * (y0 + y1));
    setMarker(new Rectangle(center, Math.abs(x1 - x0), Math.abs(y1 - y0), 
                            MARKER_ATTRIBUTES));
  }

  private void setMarker(GraphicalElement marker)
  {
    _plotCanvas.setMarker(marker);
    _plotCanvas.getGraphicsCanvas().repaint();
  }

  private void changeViewingWindow(GraphPoint point)
  {
    DataPoint p0 = _plotCanvas.getPlot().transform(_anchor);
    DataPoint p1 = _plotCanvas.getPlot().transform(point);
    double xMin = p0.getX();
    double xMax = p1.getX();
    if (xMin > xMax)
    {
      xMin = p1.getX();
      xMax = p0.getX();
    }
    double yMin = p0.getY();
    double yMax = p1.getY();
    if (yMin > yMax)
    {
      yMin = p1.getY();
      yMax = p0.getY();
    }
    setMarker(null);
    if (xMin != xMax && yMin != yMax)
    {
      setCoordinateSystem(xMin, yMin, xMax, yMax);
    }
  }
}
