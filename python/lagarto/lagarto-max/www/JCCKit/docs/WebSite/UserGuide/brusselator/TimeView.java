/*
 * Copyright 2003, Franz-Josef Elmer, All rights reserved
 */
import jcckit.util.Format;

import java.awt.*;
import java.awt.event.*;

public class TimeView extends Label {
  private final Brusselator _brusselator;
  private final Format _format = new Format("%1.2f");

  public TimeView(Brusselator brusselator) {
    _brusselator = brusselator;
    brusselator.addActionListener(new ActionListener() {
          public void actionPerformed(ActionEvent event) {
            showValue();
          }
        });
    setFont(Font.decode("SansSerif-bold-13"));
    showValue();
  }

  private void showValue() {
    setText("t = " + _format.form(_brusselator.getTime()));
    invalidate();
    if (getParent() != null) {
      getParent().validate();
    }
  }
}
