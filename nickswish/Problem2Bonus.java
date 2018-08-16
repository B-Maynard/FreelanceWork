import java.util.Scanner;

public class Problem2Bonus {

    public static double getLargestNum(double firstNum, double secondNum, double thirdNum) {


        if (secondNum < firstNum) {      //Check if the second and third doubles are less than the first
            if (thirdNum < firstNum) {
                return firstNum;        //If so, the first doubles is the largest
            }
        } else if (firstNum < secondNum) {  //Check to see if the first and third doubles are less than the second
            if (thirdNum < secondNum) {
                return secondNum;           //If so, return the second
            }
        }

        return thirdNum;     //If nothing passed, then the third double is the biggest
    }

    public static void main(String args[]) {
        double firstDouble = 0;
        double secondDouble = 0;
        double thirdDouble = 0;
        Scanner userInput = new Scanner(System.in);    //Scanner for user input

        System.out.println("Please enter three doubles separated by a space: "); //Scanner below will check the line the user inputs for separate ints as long as the doubles are separated by a space
        firstDouble = userInput.nextDouble();
        secondDouble = userInput.nextDouble();
        thirdDouble = userInput.nextDouble();


        System.out.println("Largest double: " + getLargestNum(firstDouble, secondDouble, thirdDouble)); //Print the largest
    }

}
