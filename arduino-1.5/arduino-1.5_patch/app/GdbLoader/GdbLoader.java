/**
 * Copyright (c) 2014 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 07/16/2014
 */
 
import java.io.*;
import java.util.*;

import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.Callable;
import java.util.concurrent.TimeUnit;

public class GdbLoader
{
  private enum STATUS_CODE
  {
    RUNNING, SERIAL_NOK, PROGRAMMING, COMPLETED, HALTED
  }

  private enum OS_CODE
  {
    LINUX, MACOS, WINDOWS, OTHER
  }

  private STATUS_CODE status = STATUS_CODE.RUNNING;
  
  private static OS_CODE os;
  
  private static OS_CODE getOs()
  {
    OS_CODE result;
    
    String strOs = System.getProperty("os.name").toLowerCase();
       
    if ((strOs.indexOf("mac") >= 0) || (strOs.indexOf("darwin") >= 0))
    {
      result = OS_CODE.MACOS;
    }
    else if (strOs.indexOf("win") >= 0)
    {
      result = OS_CODE.WINDOWS;
    }
    else if (strOs.indexOf("nux") >= 0)
    {
      result = OS_CODE.LINUX;
    }
    else
    {
      result = OS_CODE.OTHER;
    }
    
    return result;
  }

  /**
   * Main method
   */
	public static void main(String [ ] args)
  {
    // Detect OS
    os = getOs();

    GdbLoader uploader = new GdbLoader();
    boolean result;

    if (!(result = uploader.load(args[0], args[1], args[2])))
    {
      if (uploader.status == STATUS_CODE.HALTED)
      { 
        System.out.println("Upload failed");        
      }
      else if (uploader.status == STATUS_CODE.SERIAL_NOK)
      {
        System.out.println("Serial port not found");
      }
      System.exit(-2);
    }
    else
    {
      System.out.println("Process completed");
      System.exit(0);
    }
	}

  /**
   * Load firmware image (.elf) via msp430-gdb
   */
  public boolean load(String gdbPath, String buildPath, String serialPort)
  {
    String gdbBin;
    final String sysPrompt;

    status = STATUS_CODE.RUNNING; // Initial status;

    if (os == OS_CODE.WINDOWS)
    {
      sysPrompt = "cmd";      
    }
    else
    {
      sysPrompt = "/bin/bash";
    }

    try
    {
      final String gdbcommand = "\"" + gdbPath + "\"" + " -b 38400 " + "-ex \"target remote " + serialPort +
      "\" -ex \"set debug remote 0\" " + "\"" + buildPath + "\"" + " -ex \"erase\" -ex \"load\" -ex \"quit\"";     

      // String line;
      Process process = Runtime.getRuntime().exec(sysPrompt);

      // Kill GDB process after 5 seconds in case of programming failure
      ScheduledExecutorService scheduledExecutorService = Executors.newScheduledThreadPool(2);
      ProcessKiller pKiller = new ProcessKiller();
      final ScheduledFuture<?> scheduledFuture = scheduledExecutorService.schedule(pKiller, 5, TimeUnit.SECONDS);

      if (process != null)
      {
        BufferedReader in = new BufferedReader(new InputStreamReader(process.getInputStream()));
        PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(process.getOutputStream())),true);

        //System.out.println("Command:");
        //System.out.println(gdbcommand);

        out.println(gdbcommand);
        out.flush();
        out.close();

        boolean portFound = false;

        String sCurrentLine;
        while ((sCurrentLine = in.readLine()) != null)
        {
          System.out.println(sCurrentLine);

          if (sCurrentLine.startsWith("Reading symbols from"))
            System.out.println("Trying to contact target board...");
          else if (sCurrentLine.startsWith("Remote debugging using"))
            portFound = true;
          else if (sCurrentLine.startsWith("_reset_vector__"))
            status = STATUS_CODE.PROGRAMMING;            
        }

        process.waitFor();
        process.destroy();

        if (!portFound)
          status = STATUS_CODE.SERIAL_NOK;
      }
     
      if (status == STATUS_CODE.PROGRAMMING)
        status = STATUS_CODE.COMPLETED;
      else if (status == STATUS_CODE.RUNNING)
        status = STATUS_CODE.HALTED;
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
      if (status == STATUS_CODE.COMPLETED)
        return true;
      else
        return false;
    }
  }

  /**
   * ProcessKiller callable class. Kill msp430-gdb process
   */
  private class ProcessKiller implements Runnable
  {
    @Override public void run()
    {     
      if (status != STATUS_CODE.PROGRAMMING)
      {
        try
        {
          if (os == OS_CODE.WINDOWS)
            Runtime.getRuntime().exec("TASKKILL /F /IM msp430-gdb");
          else
            Runtime.getRuntime().exec("killall msp430-gdb");
        }
        catch (IOException e)
        {  
          e.printStackTrace();
        }
      }
    }
  }
}

