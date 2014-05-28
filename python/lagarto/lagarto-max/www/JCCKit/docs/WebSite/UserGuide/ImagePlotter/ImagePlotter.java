import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import jcckit.Graphics2DPlotCanvas;
import jcckit.data.DataPlot;
import jcckit.util.ConfigParameters;
import jcckit.util.PropertiesBasedConfigData;

/**
 * @author  Franz-Josef Elmer
 */
public class ImagePlotter
{
  public static void main(String[] args) throws IOException
  {
    System.getProperties().setProperty("java.awt.headless", "true");

    // Handles the commandline argument
    if (args.length < 3)
    {
      System.out.println("Usage: java -cp classycle.jar ImagePlotter " 
                         + "<properties file> <width> <height>");
      System.exit(0);
    }
    String propertiesFile = args[0];
    int width = Integer.parseInt(args[1]);
    int height = Integer.parseInt(args[2]);

    // Sets up a Graphics2DPlotCanvas
    ConfigParameters config
        = new ConfigParameters(new PropertiesBasedConfigData(propertiesFile));
    Graphics2DPlotCanvas plotCanvas = new Graphics2DPlotCanvas(config);
    plotCanvas.connect(DataPlot.create(config));
    plotCanvas.setDoubleBuffering(false);

    // Draws the chart into a off-screen image
    System.getProperties().setProperty("java.awt.headless", "true");
    BufferedImage image 
        = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
    plotCanvas.draw2DInto(image);

    // Writes the off-screen image into a PNG file
    ImageIO.write(image, "png", new File(propertiesFile + ".png"));
  }
}
