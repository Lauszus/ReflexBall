import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import javax.swing.JFileChooser;


/**
 * This class is used to generate an array from an ASCII art that can be put directly into the .c file.
 */
public class BackslashEscapes {

	public static void main(String[] args) throws FileNotFoundException {
		JFileChooser chooser = new JFileChooser(new File(System.getProperty("user.home"))); // Create the FileChooser instance 
		int returnVal = chooser.showOpenDialog(null); // Show the file dialog
		if(returnVal != JFileChooser.APPROVE_OPTION) // Check if user didn't press OK
			return; // If not return
		openFile(chooser.getSelectedFile().getPath());
	}
	
	private static void openFile(String filepath) throws FileNotFoundException {
		List<String> list = new ArrayList<String>();
		Scanner fileScanner = new Scanner(new File(filepath));

		while(fileScanner.hasNextLine()) // Read the entire file into the list
			list.add(fileScanner.nextLine());
		
		int height = 0;
		int width = 0;
		
		for (String line : list) { // This is used to calculate the necessary height and width of the array
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
			line = line.replaceAll("\\\\","\\\\\\\\"); // Replace \ with \\
			line = line.replaceAll("\"", "\\\\\""); // Replace " with \"
			line = line.replaceAll("'", "\\\\'"); // Replace ' with \'
			line = "\t\"" + line; // Put quota in front of string 
			
			for (int i=0;i<spaces;i++)
				line += " "; // Fill up lines with spaces, so they are all the same length
			line += "\","; // Put a quota and a comma at the end
			
			System.out.println(line);
		}
		System.out.println("};"); // And finally close the array
	}
}