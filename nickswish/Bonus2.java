/*
 * Author: Emilio Rodriguez
 * Course: CSCI 1583
 * Assignment: Homework 2
 * Date: June 13, 2018
 */

import java.util.Scanner;

public class Bonus2 {
    public static void main(String[] args) {

        Scanner input = new Scanner(System.in);

        String monsterName = "Goblin"; // Declare variable for monster's name and initialize it to “Goblin"
        int monsterHealth = 100; // Declare variable for monster's health and initialize it 100
        int monsterAttack = 15; // /Declare variable for monster's attack power and initialize it to 15
        int heroHealth = 100; // /Declare variable for Hero's health and initialize it to 100
        int heroAttack = 12; // Declare variable for Hero's attack power and initialize it to 12
        int heroMagic = 0; // /Declare variable for Hero's magic power and initialize it to 0

        //Declare loop control variable and initialize it to true
        boolean loop = true;

        while (loop == true) {

            System.out.println("\nYou are fighting a " + monsterName + "!");
            System.out.println("The monster HP: " + monsterHealth);
            System.out.println("Your HP: " + heroHealth);
            System.out.println("Your MP: " + heroMagic);

            System.out.printf("%n");
            /*Combat menu promt*/
            System.out.println("Combat Options:");
            //Print option 1: Sword Attack
            System.out.println("  1.) Sword Attack");
            //Print option 2: Cast Spell
            System.out.println("  2.) Cast Spell");
            //Print option 3: Charge Mana
            System.out.println("  3.) Charge Mana");
            //Print option 4: Run Away
            System.out.println("  4.) Run Away");
            //Prompt player for action
            System.out.print("What option do you want to perform? ");

            //Declare variable for user input (as nubmer) and acquire value from Scanner object.
            int choice;
            choice = input.nextInt();

            if (choice == 1){

                //Calculate damage & update monster health using subtraction
                monsterHealth = monsterHealth - heroAttack;

                System.out.println("\nYou strike the " + monsterName + " with your sword for " + heroAttack + " damage.");

                if (monsterHealth <= 0) {

                    loop = false; //Stop combat loop by setting control variable to false

                    System.out.println("\nYou defeated the " + monsterName + "!");
                }
                else {
                    System.out.println("\n" + monsterName + " strikes you for " + monsterAttack + " damage.");

                    heroHealth = heroHealth - monsterAttack;
                }

            }

            else if (choice == 2){

                if (heroMagic >=3){

                    //Calculate damage & update monster health using division
                    monsterHealth = monsterHealth / 2;
                    heroMagic = heroMagic - 3;

                    System.out.println("\nYou cast the weaken spell on the monster.");

                    if (monsterHealth <= 0) {

                        loop = false; //Stop combat loop by setting control variable to false

                        System.out.println("\nYou defeated the " + monsterName + "!");
                    }
                    else {
                        System.out.println("\n" + monsterName + " strikes you for " + monsterAttack + " damage.");

                        heroHealth = heroHealth - monsterAttack;
                    }

                }

                else {

                    System.out.println("You don't have enough mana.");
                }
            }

            else if (choice == 3){

                //Increment magic points and update players magic using addition
                heroMagic = heroMagic + 1;

                System.out.println("\nYou focus and charge your magic power.");

            }

            else if (choice == 4){

                //Stop combat loop by setting control variable to false
                loop = false;

                System.out.println("\nYou run away!");
            }

            else {

                System.out.println("\nI don't understand that command.");
            }

            if (heroHealth <= 0) {

                loop = false;

                System.out.println("\nSorry, the monster has defeated you...");
            }

        } // end of while loop

    } // end of main

} // end of Bonus2