package Socket_Communication;
import java.net.*;
import java.io.*;

public class Dateserver{
    public static void main(String[] args){
        try{
            ServerSocket sock = new ServerSocket(6013);
            /*Listening connection*/
            while(true){
                Socket client = sock.accept();

                PrintWriter pout = new
                 PrintWriter(client.getOutputStream(), true);
                 /*Write Data to socket*/ 
                 pout.println(new java.util.Date().toString());

                 /*close socket*/
                 /*Listening connection*/
                 client.close();
            }
        }
        catch (IOException ioe){
            System.err.println(ioe);
        }
    }
}