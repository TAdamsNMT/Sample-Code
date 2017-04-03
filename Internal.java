
public class Internal {
	final int MINE = -1;
	//Mines will be handled in the numbers grid; a space holds -1 iff it is a mine.
	
	int numMines;
	int totalMines;
	int numFlags;
	boolean boom;
	boolean[][] flags;
	boolean[][] revealed;
	int[][] numbers;
	
	public Internal(int gridSize, int numMines)
	{
		flags = new boolean[gridSize][gridSize];
		revealed = new boolean[gridSize][gridSize];
		numbers = new int[gridSize][gridSize];
		this.numMines = numMines;
		totalMines = numMines;
		
		for(int i = 0; i < numMines; i++)
		{
			addMine();
		}
		
	}
	
	private void addMine()
	{
		int x = (int) (Math.random() * gridSize());
		int y = (int) (Math.random() * gridSize());
		
		if(numbers[x][y] != MINE)
			numbers[x][y] = MINE;
		else
		{
			addMine();
			return;
		}
		
		
		for(int dx = -1; dx <=1; dx++)
			for(int dy = -1; dy <= 1; dy++)
				increment(x + dx, y + dy);
	}
	
	private void increment(int x, int y)
	{
		if(x < 0) return;
		if(x >= gridSize()) return;
		if(y < 0) return;
		if(y >= gridSize()) return;
		if(numbers[x][y] == MINE) return;
		
		numbers[x][y]++;
	}
	
	public String whatToPrint(int x, int y)
	{
		if(flags[x][y] && boom && numbers[x][y] != MINE) return "F" + numbers[x][y];
		if(flags[x][y]) return "F";
		if(!revealed[x][y] && !boom) return "?";
		if(numbers[x][y] == MINE) return "X";
		if(numbers[x][y] == 0)	return "";
		return "" + numbers[x][y];
	}
	
	public int gridSize()
	{
		return numbers.length;
	}
	
	public boolean reveal(int x, int y)
	{
		if(boom) return true;
		
		if(x < 0) return false;
		if(x >= gridSize()) return false;
		if(y < 0) return false;
		if(y >= gridSize()) return false;
		
		if(flags[x][y]) return false;
		if(revealed[x][y]) return false;
		
		revealed[x][y] = true;
		
		if(numbers[x][y] == 0)
			return revealGroup(x,y);
		
		if(numbers[x][y] == MINE) boom = true;
				
		return boom;
	}
	
	public boolean checkWin()
	{
		//if any non-mine space isn't revealed or any mine space is revealed, return false
		for(int i = 0; i < gridSize(); i++)
			for(int j = 0; j < gridSize(); j++)
				if((numbers[i][j] == MINE) == revealed[i][j])	
					return false;
		return true;
	}
	
	public void flag(int x, int y)
	{
		if(revealed[x][y]) return;
		
		if(flags[x][y])
			numMines++;
		else
			numMines--;
		
		flags[x][y] = !flags[x][y];
	}
	
	public boolean revealGroup(int x, int y)
	{
		boolean boom = false;
		for(int dx = -1; dx <=1; dx++)
			for(int dy = -1; dy <= 1; dy++)
				boom = boom || reveal(x + dx, y + dy);
		
		return boom;
	}
	
	public int getMines()
	{
		return totalMines;
	}
	
	public int getCounter()
	{
		return numMines;
	}
}
