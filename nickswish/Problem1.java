import java.util.Scanner;

public class Problem1 {

    public static int numNickels(double dollarValue){
        double nickels = 0;

        dollarValue = dollarValue * 100;    //Make the dollar value a whole number so that dividing is more easily done
        nickels = dollarValue / 5; //nickels will automatically round to a whole number

        return (int)nickels;    //Return nickels typecasted as an int
    }



    public static void main(String[] args) {
        int nickels = 0;
        double dollarValue = 0.0;
        Scanner reader = new Scanner(System.in);    //Create scanner to read user input

        System.out.println("Enter dollar value(must be in double format): ");   //Prompt user to input dollar value in double format
        dollarValue = reader.nextDouble();  //Read the double the user inputs
        reader.close(); //Close the reader

        nickels = numNickels(dollarValue);  //Run the function built above on the users' dollar value

        System.out.println("Number of nickels from $" + dollarValue + ": " + nickels);   //Print the number of nickels
    }
}
