/*
 * Copyright 2003, Franz-Josef Elmer, All rights reserved
 */
import java.awt.*;
import java.awt.event.*;

public class Brusselator {
  private double _noiseFactor = 0.001;
  private double _alpha, _beta, _d;
  private double _time;
  private final double[] _u, _v, _du, _dv;
  private final double _dx;
  private final double _dt;
  private ActionListener _listener;

  public Brusselator(double dx, double dt, double size) {
    _dx = dx;
    _dt = dt;
    _u = new double[(int) (size / dx + 0.5)];
    _v = new double[_u.length];
    _du = new double[_u.length];
    _dv = new double[_u.length];
    reset();
    addNoise();
  }

  public void addActionListener(ActionListener listener) {
    _listener = AWTEventMulticaster.add(_listener, listener);
  }

  private void notifyListeners(ActionEvent event) {
    if (_listener != null) {
      _listener.actionPerformed(event);
    }
  }

  public double getDx() {
    return _dx;
  }

  public double getDt() {
    return _dt;
  }

  public double getTime() {
    return _time;
  }

  public void setTime(double time) {
    _time = time;
  }

  public void setNoiseFactor(double factor) {
    _noiseFactor = factor;
  }

  public void setAlpha(double alpha) {
    _alpha = alpha;
  }

  public void setBeta(double beta) {
    _beta = beta;
  }

  public void setD(double d) {
    _d = d;
  }

  public int getNumberOfPoints() {
    return _u.length;
  }

  public double getU(int index) {
    return _u[index];
  }

  public void setU(int index, double value) {
    _u[index] = value;
  }

  public double getV(int index) {
    return _v[index];
  }

  public void setV(int index, double value) {
    _v[index] = value;
  }

  public void reset() {
    for (int i = 0; i < _u.length; i++) {
      _u[i] = _beta;
      _v[i] = _alpha / Math.max(0.01, _beta);
    }
    _time = 0;
    notifyListeners(new ActionEvent(this, 0, "reset"));
  }

  public void addNoise() {
    for (int i = 0; i < _u.length; i++) {
      _u[i] *= 1 + (0.5 - Math.random()) * _noiseFactor;
      _v[i] *= 1 + (0.5 - Math.random()) * _noiseFactor;
    }
    _time = 0;
    notifyListeners(new ActionEvent(this, 0, "state changed"));
  }

  public void next() {
    int steps = (int) (_dt / ((_dx * _dx) / (2.5 * Math.max(1, _d)))) + 1;
    double dt = _dt / steps;
    for (int k = 0; k < steps; k++) {
      for (int i = 0, n = _u.length; i < n; i++) {
        _du[i] = (_u[(i + n - 1) % n] - 2 * _u[i] + _u[(i + 1) % n]) / (_dx * _dx)
                 + _beta + _u[i] * (_u[i] * _v[i] - 1 - _alpha);
        _dv[i] = _d * (_v[(i + n - 1) % n] - 2 * _v[i] + _v[(i + 1) % n]) / (_dx * _dx)
                 + _u[i] * (_alpha - _u[i] * _v[i]);
      }
      // Euler schema
      for (int i = 0, n = _u.length; i < n; i++) {
        _u[i] += dt * _du[i];
        _v[i] += dt * _dv[i];
      }
    }
    _time += _dt;
    notifyListeners(new ActionEvent(this, 0, "state changed"));
  }

}
