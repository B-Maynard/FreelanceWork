import java.util.Scanner;
import java.util.Random;

public class AdventureGame1 {
    //The users' game over variable. If they decide to quit, this will turn true.
    private static boolean gameOver = false;
    private static int gameWinCheck = 0;

    //main executes game loop
    public static void main(String [] args) {
        gameStart();
    }

    public static void gameStart() {

        Scanner userInput = new Scanner(System.in);

        //asks the user for the player object's name
        System.out.println("Please enter your character's first name: ");
        String playerName = userInput.nextLine();

        //Constructor for the player object
        Player player = new Player(playerName, 60, 5, 0);
        //start message
        System.out.println("+-------------------+");
        System.out.println("Welcome to your adventure, " + player.getName() + "! The game begins in the cockpit of your spaceship. Everything was going fine, until your ship was invaded with aliens!");
        System.out.println("The aliens are set out to destroy anything in their path, so it's your job to clear the aliens out of your spaceship.");
        System.out.println("To beat the game, you have to defeat a total of 10 aliens that are on your ship. Good luck!");
        System.out.println("+-------------------+");




        Random rand = new Random();
        int enemyCheck = 0;

        //initialize dungeon with basic constructor
        Dungeon map = new Dungeon();

        //make current room the starting room
        Room currentRoom = map.getStartingRoom();

        //make newRoom variable the currentRoom so loop can tell what room we are in
        Room newRoom = currentRoom;

        //Variables to check for enemy and item occurrence in each room
        //Also establishes variables for checking which items the player puts in their inventory and whether or not the player died in battle
        boolean enemyToggle = false;
        boolean itemToggle = false;
        int runCheck = 0;
        int itemId = 0;
        while(!gameOver) {

            //if the player is able to encounter an enemy, run a random number from 1 to 10
            if (enemyToggle) {
                enemyCheck = rand.nextInt(10) + 1;
            }

            //If the number generated above is greater than 4, encounter an enemy
            if (enemyCheck > 4 && enemyToggle) {
                runCheck = engageCombat(player);

                //if runCheck is 1, it means the player died in battle
                if (runCheck == 1) {
                    break;
                }
                //Will not let you encounter an enemy in back-to-back rooms, to avoid constantly fighting
                enemyToggle = false;
            }

            //if the random number generated is between 1 and 2, spawn a random item
            else if (enemyCheck >= 1 && enemyCheck <= 3 && itemToggle) {
                //Retrieve the random item's itemID
                itemId = currentRoom.setItems();

                //If the player has chosen to keep the item, add it to their inventory
                if (itemId != 0) {
                    player.addToInventory(itemId);
                }
            }

            //If the player defeated 10 enemies, they win the game
            if (gameWinCheck == 10) {
                System.out.println("Congrats! You've successfully beaten the game!");
                gameOver = true;
                break;
            }

            //checks to make sure the room you're trying to change into is a null or not.
            //if it is null, the player will not change rooms
            //If runCheck is code 4, it means the player decided to run from battle, so don't go to a new room
            if (newRoom != null && runCheck != 4) {
                currentRoom = newRoom;
                enemyToggle = true;
                itemToggle = true;
            }

            //Re-establish runCheck to 0 so that the player is free to move around again
            runCheck = 0;
            System.out.printf("You are in: %s. Where would you like to go?", currentRoom.getDescription());
            System.out.print(currentRoom.getExits());
            System.out.println("\n(Enter command. Press 'i' to see your inventory): ");


            String choice = userInput.next();

            /* This list of if statements all do the same thing. Each will check their specific direction.
             They will then set the newRoom variable to whatever room the player chooses. If the player chooses an invalid room,
             the variable will be set to null. If it is not null, the newRoom will be set to the valid room choice. */
            if (choice.equalsIgnoreCase("n")) {
                newRoom = currentRoom.getNorth();
                if (newRoom == null) {
                    System.out.println("There is no exit in that direction. Please choose a different path.");
                    enemyToggle = false;
                    itemToggle = false;
                }
            }
            else if (choice.equalsIgnoreCase("e")) {
                newRoom = currentRoom.getEast();
                if (newRoom == null) {
                    System.out.println("There is no exit in that direction. Please choose a different path.");
                    enemyToggle = false;
                    itemToggle = false;
                }
            }
            else if (choice.equalsIgnoreCase("w")) {
                newRoom = currentRoom.getWest();
                if (newRoom == null) {
                    System.out.println("There is no exit in that direction. Please choose a different path.");
                    enemyToggle = false;
                    itemToggle = false;
                }
            }
            else if (choice.equalsIgnoreCase("s")) {
                newRoom = currentRoom.getSouth();
                if (newRoom == null) {
                    System.out.println("There is no exit in that direction. Please choose a different path.");
                    enemyToggle = false;
                    itemToggle = false;
                }
            }
            //If player chooses to quit the game, gameOver becomes true and an exit message is shown
            else if (choice.equalsIgnoreCase("q")) {
                gameOver = true;
                System.out.println("You blast off into the distance to explore the universe! Best of luck traveller!");
            }
            //If player chooses i, it will allow them to manipulate their inventory
            else if (choice.equalsIgnoreCase("i")) {
                enemyToggle = false;
                itemToggle = false;
                player.accessInventory();
                player.equipItems();
            }
            //if the player does input that does not match the above options, print an error message
            else {
                System.out.println("That is not a valid option.");
            }
        }
    }

    //Function loop that will engage the player in combat with enemies
    public static int engageCombat(Player player) {
        Scanner input = new Scanner(System.in);
        //Establish the monster object
        Monster alien = new Monster("alien", 40, 4, 100);
        int itemId;



		while (true) {
		    //Starter text
            System.out.println("+------------------+");
            System.out.println("MONSTER ENCOUNTER");
            System.out.println("+------------------+");
            System.out.println("Monster: " + alien.getName());
            System.out.println("Monster Health: " + alien.getHealth());
            System.out.println("Your health: " + player.getHealth());
            System.out.println("Your mana: " + player.getMana());

			System.out.printf("%n");
			/*Combat menu prompt*/
			System.out.println("Combat Options:");
			//Print option 1: Sword Attack
			System.out.println("  1.) Sword Attack");
			//Print option 2: Cast Spell
			System.out.println("  2.) Cast Spell");
			//Print option 3: Charge Mana
			System.out.println("  3.) Charge Mana");
			//Print option 4: Run Away
			System.out.println("  4.) Run Away");
			//Print option 5: View inventory
            System.out.println("  5.) View Inventory");
			//Prompt player for action
			System.out.print("What option do you want to perform? ");



			//Declare variable for user input (as number) and acquire value from Scanner object.
			String choice;
			choice = input.next();

			//If the player chose to run from battle, return code 4
            if (choice.equalsIgnoreCase("4")) {
                return 4;
            }

            //Otherwise, let the player take their turn
			player.takeTurn(choice, alien);
            //If monster health reaches less than or equal to 0, you've beaten it
			if (alien.getHealth() <= 0) {
			    System.out.println("You have defeated the monster! You have gained " + alien.getXP() + " experience!");
			    itemId = alien.setItems();

			    //Enemies drop items every time they're defeated
			    //If the player wants the item, add it to the inventory
			    if (itemId != 0) {
			        player.addToInventory(itemId);
                }
                //Increment counter that checks if the player has beaten the game yet
			    gameWinCheck++;
			    return 0;
            }
            //Let the monster take its turn
            alien.takeTurn(player);
			if (player.getHealth() <= 0) {
			    System.out.println("You have died! Game over!");
			    return 1;
            }

		} // end of while loop
    }
}
