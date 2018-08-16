import java.util.Scanner;

public class Homework4 {

    private static final int NORTH = 0;
    private static final int EAST = 1;
    private static final int WEST = 2;
    private static final int SOUTH = 3;

    private static final int NUMBER_OF_ROOMS = 8;

    private static String[] description =

            /* Descriptions for the rooms */

            {
                    "Cockpit.\nThis is where you pilot the spacecraft.\nYou can exit to the North, East, or West.",
                    "Left Hall.\nThis connects the Cockpit and the Captain's Bedroom.\nYou can exit to the East or South.",
                    "Captain's Bedroom.\nThis is typically where the Captain sleeps.\nYou can exit to the North.",
                    "Right Hall.\nThis connects the Cockpit and the Co-Captain's bedroom.\nYou can exit to the West or South.",
                    "Co-Captain's Bedroom.\nThis is where the Co-Captain would sleep, if you had one...\nYou can exit to the North.",
                    "Back of the Ship.\nYou tend to stay away from here, as it gets very dark and creepy.\nYou can exit to the South, East, or West.",
                    "Storage Room.\nCareful, the door locks behind you.\nYou can exit to the East.",
                    "Escape Pods.\nThe escape pods are out of order. How safe!\nYou can exit the the West."
            };

    private static int [] [] exit =

            /* The 2D array that determines the exits for each room.
                EX: Room 1 is the first line, and each number in the first line represents a cardinal direction (North, East, West, and South).
             */

            {
                    { 5, 3, 1, -1},
                    { -1, 0,  -1, 2},
                    { 1, -1, -1, -1},
                    {-1, -1, 0,  4},
                    {3, -1, -1, -1},
                    {-1,  7,  6,  0},
                    {-1, 5, -1,  -1},
                    {-1, -1, 5, -1},
            };

    private static int currentRoom = 0;
    private static boolean gameLoop = true;

    public static void main (String [] args) {
        directionToGo();
    }

    public static void escapeRoom(final int PATH) {

        /* If the user picks a direction to go that is not possible, spit error. Else, go the direction specified. */

        if (exit[currentRoom][PATH] != -1) {
            currentRoom = exit[currentRoom][PATH];
        }
        else {
            System.out.println("You hit a wall, take another route.");
        }
    }

    public static void directionToGo() {
        while (gameLoop) {

            /* While the gameLoop var is true, loop through and let the user choose directions.
                User can use commands "n", "s", "e", or "w", which are case insensitive.
                User can also use "q" command to quit game.
             */

            System.out.printf("You are in: %s", description[currentRoom]);
            System.out.println();
            Scanner input = new Scanner(System.in);

            System.out.println("Which direction do you choose?");

            String direction = input.next();

            if (direction.equals("N") || direction.equals("n")) {
                escapeRoom(NORTH);
            }
            else if (direction.equals("E") || direction.equals("e")) {
                escapeRoom(EAST);
            }
            else if (direction.equals("S") || direction.equals("s")) {
                escapeRoom(SOUTH);
            }
            else if (direction.equals("W") || direction.equals("w")) {
                escapeRoom(WEST);
            }
            else if (direction.equals("Q") || direction.equals("q")) {
                gameLoop = false;
                System.out.println("You take hold of the Cockpit and return to Earth.");
            }
            else {
                System.out.println("Input is invalid.");
            }
        }
    }

}
