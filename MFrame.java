import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class MFrame extends JFrame
{
	
	MPanel panel;
	CounterPanel cpanel;
	
	public static void main(String[] args)
	{
		new MFrame();
	}
	
	public MFrame() {
		setTitle("Minesweeper");
		setSize(500,500);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setLayout(new BorderLayout(0, 10));
		JMenuBar mb = new JMenuBar();
		JMenu menu = new JMenu("Game");
		mb.add(menu);
		setJMenuBar(mb);
		menu.add(new RestartMenuItem());
		menu.add(new ResetMenuItem());
		panel = new MPanel(this);
		add(panel, BorderLayout.CENTER);
		cpanel = new CounterPanel(panel.getInternal());
		add(cpanel, BorderLayout.NORTH);
		setVisible(true);
	}
	
	public void refresh()
	{
		remove(cpanel);
		cpanel = new CounterPanel(panel.getInternal());
		add(cpanel, BorderLayout.NORTH);
		repaint();
	}
	
	private class CounterPanel extends JPanel
	{
		Internal game;
		JLabel cLabel;
		
		public CounterPanel(Internal game)
		{
			this.game = game;
			cLabel = new JLabel("", JLabel.CENTER);
			cLabel.setText(game.getCounter() + "");
			add(cLabel);
		}
		
		public void paint(Graphics g)
		{
			cLabel.setText(game.getCounter() + "");
			super.paint(g);
		}
	}

	
	private class RestartMenuItem extends JMenuItem
	{
		public RestartMenuItem()
		{
			super("Restart");
			addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					int reply = JOptionPane.showConfirmDialog(null, "Are you sure you want to start a new game \nwith the same grid size and amount of mines?");
					if(reply == JOptionPane.YES_OPTION)
					{panel.restart();}
					
				}
			});
		}
	}
	
	private class ResetMenuItem extends JMenuItem
	{
		public ResetMenuItem()
		{
			super("Reset");
			addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					int reply = JOptionPane.showConfirmDialog(null, "Are you sure you want to start a new game?");
					if(reply == JOptionPane.YES_OPTION)
					{ panel.reset(); }
				}
			});
		}
	}
}
