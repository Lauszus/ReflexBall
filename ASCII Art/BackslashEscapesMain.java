import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import javax.swing.JFileChooser;


public class BackslashEscapesMain {

	/**
	 * @param args
	 * @throws FileNotFoundException 
	 */
	public static void main(String[] args) throws FileNotFoundException {
		JFileChooser chooser = new JFileChooser(new File(System.getProperty("user.home"))); // Create the FileChooser instance 
		int returnVal = chooser.showOpenDialog(null);
		if(returnVal != JFileChooser.APPROVE_OPTION)
			return;
		openFile(chooser.getSelectedFile().getPath());
	}
	
	private static void openFile(String filepath) throws FileNotFoundException {
		List<String> list = new ArrayList<String>();
		Scanner fileScanner = new Scanner(new File(filepath));

		while(fileScanner.hasNextLine())
			list.add(fileScanner.nextLine());
		
		int height = 0;
		int width = 0;
		
		for (String line : list) {
			System.out.println(line);
			height++;
			line = line.replaceAll("\\s+$", ""); // Remove spaces to the right
			if (width < line.length())
				width = line.length();
		}
		
		System.out.println("\nrom const char name[" + height + "][" + (width+1) + "] = {"); // Make room for null-character
		for (String line : list) {
			int spaces = 0;			
			line = line.replaceAll("\\s+$", ""); // Remove spaces to the right
			for (int i=line.length();i<width;i++)
				spaces++;
			line = line.replaceAll("\\\\","\\\\\\\\");
			line = line.replaceAll("\"", "\\\\\"");
			line = line.replaceAll("'", "\\\\'");
			line = "\t\"" + line;
			
			for (int i=0;i<spaces;i++)
				line += " ";
			line += "\",";
			
			System.out.println(line);
		}
		System.out.println("};");
	}

}
