/////////////////////////////////////////////////////////////////////
// Hack at Global Day of Code Retreat 2016 - Hamburg 
// code by Gunther Bachman
//



package gol;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Playground {

	int[][] cells;
	int MAXX;
	int MAXY;

	public Playground(int maxx, int maxy) {
		MAXX = maxx;
		MAXY = maxy;
		cells = new int[maxx][maxy];
	}

	public boolean isAlive() {
		for (int x = 0; x < MAXX; x++) {
			for (int y = 0; y < MAXY; y++) {
				if (getCell(x, y) > 0) {
					return true;
				}
			}
		}
		return false;
	}

	public void setCell(int x, int y, int status) {
		cells[x][y] = status;
	}

	public int getCell(int x, int y) {
		return cells[x][y];
	}

	/** determine next generation status of cell */
	public int determineNewCellStatus(int x, int y) {
		List<Integer[]> neighbors = determineNeighbors(x, y);

		int aliveNeighbors = 0;
		for (Integer[] pair : neighbors) {
			aliveNeighbors += getCell(pair[0], pair[1]);
		}

		int oldStatus = getCell(x, y);

		if (aliveNeighbors == 3) {
			return 1;
		}
		if (aliveNeighbors == 2 && oldStatus == 1) {
			return 1;
		}

		return 0;
	}

	public void initField(String... strings) {
		for (int y = 0; y < strings.length; y++) {
			String line = strings[y];
			char[] charArray = line.toCharArray();
			for (int x = 0; x < charArray.length; x++) {
				if (charArray[x]== ' ') {
					setCell(x, y, 0);
				} else {
					setCell(x, y, 1);
				}
			}
		}
	}
	
	public Playground nextPlayground() {
		Playground result=new Playground(MAXX, MAXY);
		for(int x=0;x<MAXX;x++) {
			for(int y=0;y<MAXY;y++) {
				result.setCell(x,y,determineNewCellStatus(x, y));
			}
		}
		return result;
	}
	
	public List<String> toStrings() {
		List<String> result=new ArrayList<String>();
		for(int y=0;y<MAXY;y++) {
			StringBuffer sb=new StringBuffer();
			for( int x=0;x<MAXX;x++) {
				if(getCell(x,y)==0){
					sb.append(' ');
				}else{
					sb.append('*');
				}
			}
			result.add(sb.toString());
		}
		return result;
	}
	
	public void print() {
		for(String str:toStrings()){
			System.out.println(str);
		}		
	}

	public List<Integer[]> determineNeighbors(int x, int y) {
		List<Integer[]> result = new ArrayList<Integer[]>();
		result.add((Integer[]) (Arrays.asList((x - 1 + MAXX) % MAXX,
				(y - 1 + MAXY) % MAXY).toArray()));
		result.add((Integer[]) (Arrays.asList((x - 1 + MAXX) % MAXX, y)
				.toArray()));
		result.add((Integer[]) (Arrays.asList((x - 1 + MAXX) % MAXX, (y + 1)
				% MAXY).toArray()));

		result.add((Integer[]) (Arrays.asList(x, (y - 1 + MAXY) % MAXY)
				.toArray()));
		result.add((Integer[]) (Arrays.asList(x, (y + 1) % MAXY).toArray()));

		result.add((Integer[]) (Arrays.asList((x + 1) % MAXX, (y - 1 + MAXY)
				% MAXY).toArray()));
		result.add((Integer[]) (Arrays.asList((x + 1) % MAXX, y).toArray()));
		result.add((Integer[]) (Arrays.asList((x + 1) % MAXX, (y + 1) % MAXY)
				.toArray()));

		return result;
	}

}
