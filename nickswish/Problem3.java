public class Problem3 {

    public static void main(String args[]) {

        for (int i = 0; i <= 100; i++) {    //Loop from 0 to 100
            if (i % 2 != 0) {   //Checking if i modulus 2 == 0 checks to see if the current number, i, is divisible by 2. If it isn't, it's odd
                System.out.println("\n" + i);   //If it's odd, print it
            }
        }

    }
}
