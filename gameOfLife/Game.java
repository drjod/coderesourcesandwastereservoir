/////////////////////////////////////////////////////////////////////
// Hack at Global Day of Code Retreat 2016 - Hamburg 
// code by Gunther Bachman
//

package gol;

public class Game {
	public static void main(String[] args) {
		Playground playground=new Playground(20,20);
		/*playground.initField(//
				" *        ",//
				"  *       ",//
				"***       ",//
				"          ",//
				"          ",//
				"          ",//
				"          ",//
				"          ",//
				"          ",//
				"          "//
				);
		*/
		playground.initField(//
				" ",//
				" ",//
				" ",//
				" ",//
				" ",//
				"          ***",//				
				"          * *",//	
				"          * *",//	
				"         ",//		
				"          * *",//	
				"          * *",//	
				"          ***");//	
				
		while(playground.isAlive()){
			playground.print();
			try {
				Thread.sleep(1000);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Playground newPlayground=playground.nextPlayground();
			playground=newPlayground;
		}
	}
}
