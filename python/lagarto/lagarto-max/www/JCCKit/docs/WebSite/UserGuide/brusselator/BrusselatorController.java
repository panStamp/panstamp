/*
 * Copyright 2003, Franz-Josef Elmer, All rights reserved
 */
import java.awt.*;
import java.awt.event.*;

public class BrusselatorController {
  private Brusselator _brusselator;
  private Thread _animationThread;

  public BrusselatorController(Brusselator brusselator) {
    _brusselator = brusselator;
  }

  public Panel getControlPanel() {
    Panel result = new Panel();
    Button startButton = new Button("start");
    startButton.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent event) {
            _animationThread = new Thread() {
                  public void run() {
                    while (_animationThread != null) {
                      try {
                        Thread.sleep(100);
                      } catch (InterruptedException e) {}
                      _brusselator.next();
                    }
                  }
                };
            _animationThread.start();
          }
        });
    result.add(startButton);
    Button stopButton = new Button("stop");
    stopButton.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent event) {
            _animationThread = null;
          }
        });
    result.add(stopButton);
    Button resetButton = new Button("reset");
    resetButton.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent event) {
            _brusselator.reset();
          }
        });
    result.add(resetButton);
    Button addNoiseButton = new Button("add noise");
    addNoiseButton.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent event) {
            _brusselator.addNoise();
          }
        });
    result.add(addNoiseButton);

    return result;
  }
}
