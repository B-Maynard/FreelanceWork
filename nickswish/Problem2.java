import java.util.Scanner;

public class Problem2 {

    public static int getLargestNum(int firstNum, int secondNum, int thirdNum) {


       if (secondNum < firstNum) {      //Check if the second and third ints are less than the first
            if (thirdNum < firstNum) {
                return firstNum;        //If so, the first int is the largest
            }
        } else if (firstNum < secondNum) {  //Check to see if the first and third ints are less than the second
            if (thirdNum < secondNum) {
                return secondNum;           //If so, return the second
            }
        }

       return thirdNum;     //If nothing passed, then the third int is the biggest
    }

    public static void main(String args[]) {
        int firstInt = 0;
        int secondInt = 0;
        int thirdInt = 0;
        Scanner userInput = new Scanner(System.in);    //Scanner for user input

        System.out.println("Please enter three numbers separated by a space: "); //Scanner below will check the line the user inputs for separate ints as long as the ints are separated by a space
        firstInt = userInput.nextInt();
        secondInt = userInput.nextInt();
        thirdInt = userInput.nextInt();


        System.out.println("Largest int: " + getLargestNum(firstInt, secondInt, thirdInt)); //Print the largest
    }

}
