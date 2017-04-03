import javax.swing.*;

import java.awt.*;
import java.awt.event.*;

public class MPanel extends JPanel
{
	Internal game;
	boolean firstClick = true;
	MouseHandler mH;
	MFrame frame;
	
	public MPanel(MFrame frame) 
	{
		this.frame = frame;
		game = initialize();
		mH = new MouseHandler(); 
		for(int i = 0; i < game.gridSize(); i++)
			for(int j = 0; j < game.gridSize(); j++)
			{
				GridButton gb = new GridButton(i,j);
				gb.addMouseListener(mH);
				add(gb);
			}
	}
	
	public Internal getInternal()
	{
		return game;
	}
	
	public void reset()
	{
			game = initialize();
			refreshButtons();
	}
	
	public void refreshButtons()
	{
		removeAll();
		for(int i = 0; i < game.gridSize(); i++)
			for(int j = 0; j < game.gridSize(); j++)
			{
				GridButton gb = new GridButton(i,j);
				gb.addMouseListener(mH);
				add(gb);
			}
		revalidate();
		frame.refresh();
	}
	
	private void boom()
	{
		JOptionPane.showMessageDialog(null, "BOOM!");
		refreshButtons();
		endDialog();
	}	
	
	private void win()
	{
		JOptionPane.showMessageDialog(null, "You Won!");
		refreshButtons();
		endDialog();
	}
	
	private Internal initialize()
	{
		firstClick = true;
		
		String gs = JOptionPane.showInputDialog(null, "What length should the sides of the grid be? "
				+ "\nPlease enter a number greater than 1.");
		int gridSize = -1;
		try
		{
			gridSize = Integer.parseInt(gs);
		}
		catch(NumberFormatException e)
		{
			int reply = JOptionPane.showConfirmDialog(null, "That was not a valid grid size.  "
					+ "\nPlease enter a number greater than 1");
			
			if(reply != JOptionPane.YES_OPTION)
				System.exit(0);
			
			return initialize();
		}
		if(gridSize <=1)
		{
			int reply = JOptionPane.showConfirmDialog(null, "That was not a valid grid size.  "
					+ "\nPlease enter a number greater than 1");
			
			if(reply != JOptionPane.YES_OPTION)
				System.exit(0);
			
			return initialize();
		}
		
		
		String mines = JOptionPane.showInputDialog(null, "How many mines?"
				+ "\nPlease enter a number greater than 0 "
				+ "\nand less than the number of spaces in the grid");
		int numMines = -1;
		try
		{
			numMines = Integer.parseInt(mines);
		}
		catch(NumberFormatException e)
		{
			int reply = JOptionPane.showConfirmDialog(null, "That was not a valid number of mines.  Please enter a number greater than 0 \n "
					+ "and less than the number of spaces in the grid");
			
			if(reply != JOptionPane.YES_OPTION)
				System.exit(0);
			
			return initialize();
		}
		if(numMines <=0 || numMines >= gridSize * gridSize)
		{
			int reply = JOptionPane.showConfirmDialog(null, "That was not a valid number of mines.  Please enter a number greater than 0 \n "
					+ "and less than the number of spaces in the grid");
			
			if(reply != JOptionPane.YES_OPTION)
				System.exit(0);
			
			return initialize();
		}
		
		setLayout(new GridLayout (0, gridSize));
		
		return new Internal(gridSize, numMines);
	}
	
	public void restart()
	{
			firstClick = true;
			game = new Internal(game.gridSize(), game.getMines());
			refreshButtons();
	}
	
	
	private void endDialog()
	{
		Object[] options = {"Restart", "Reset", "Exit"};
		int reply = JOptionPane.showOptionDialog(null, "Would you like to Restart with this grid size and mine count or Reset to different values?",
				"Replay?", JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, null, options, options[2]);
		
		if(reply == JOptionPane.YES_OPTION) restart();
		if(reply == JOptionPane.NO_OPTION) reset();
		if(reply == JOptionPane.CANCEL_OPTION) System.exit(0);
	}
	private class GridButton extends JButton
	{
		public int x;
		public int y;
		
		public GridButton(int x, int y)
		{
			super(game.whatToPrint(x,y));
			this.x = x;
			this.y = y;
			String s = game.whatToPrint(x, y);
			
			if(s.equals("1"))
				setForeground(Color.BLUE);
			if(s.equals("2"))
				setForeground(Color.GREEN);
			if(s.equals("3"))
				setForeground(Color.RED);
			if(s.equals("4"))
				setForeground(Color.PINK);
			if(s.equals("5"))
				setForeground(Color.GRAY);
			if(s.equals("6"))
				setForeground(Color.CYAN);
			if(s.equals("7"))
				setForeground(Color.MAGENTA);
			if(s.equals("8"))
				setForeground(Color.ORANGE);
		}
		
		public int getx()
		{
			return x;
		}
		
		public int gety()
		{
			return y;
		}
	}
	
	private class MouseHandler extends MouseAdapter
	{
		
		public void mouseClicked(MouseEvent e)
		{
			int mButton = e.getButton();
			GridButton gButton = (GridButton) e.getComponent();
			if(mButton == MouseEvent.BUTTON1){
				if(game.reveal(gButton.getx(),gButton.gety()))
				{
					if(!firstClick)
						boom();
					else
					{
						restart();
						mouseClicked(e);
						return;
					}
				}
				firstClick = false;
				refreshButtons();
				if(game.checkWin())
					win();
			}
			
			if(mButton == MouseEvent.BUTTON2){
				if(game.revealGroup(gButton.getx(),gButton.gety()))
					boom();
				refreshButtons();
				if(game.checkWin())
					win();
			}
			
			if(mButton == MouseEvent.BUTTON3)
				game.flag(gButton.getx(),gButton.gety());
			refreshButtons();
		}
	}
	
	

}
