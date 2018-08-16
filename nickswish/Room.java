import java.util.Random;
import java.util.Scanner;

public class Room {


    //Private variables for setting up room directions
    private String roomDescription;
    private Room north;
    private Room east;
    private Room west;
    private Room south;

    //constructor
    public Room(String roomDescription) {
        this.roomDescription = roomDescription;
    }


    //setters for the rooms' directions
    public void setNorth(Room north) {
        this.north = north;
    }
    public void setEast(Room east) {
        this.east = east;
    }
    public void setWest(Room west) {
        this.west = west;
    }
    public void setSouth(Room south) {
        this.south = south;
    }

    //sets the exits for each individual room.
    public void setExits(Room n, Room e, Room w, Room s) {
        this.north = n;
        this.east = e;
        this.west = w;
        this.south = s;
    }

    //our getters for each room. Allows us to traverse from room to room.
    public Room getNorth() {
        return this.north;
    }
    public Room getEast() {
        return this.east;
    }
    public Room getWest() {
        return this.west;
    }
    public Room getSouth() {
        return this.south;
    }


    //returns the description of specific room
    public String getDescription() {
        return this.roomDescription;
    }

    //returns all the exits for a specific room
    public String getExits() {
        String exit="";

        if (this.north != null) {
            exit += "\nn: The " + north.getDescription() + " is to the North!";
        }
        if (this.east != null) {
            exit +="\ne: The " + east.getDescription() + " is to the East!";
        }
        if (this.west != null) {
            exit += "\nw: The " + west.getDescription() + " is to the West!";
        }
        if (this.south != null) {
            exit += "\ns: The " + south.getDescription() + " is to the South!";
        }

        return exit;
    }

    //returns the room description and the exits for a specific room
    public String toString() {
        return this.roomDescription + "\n" + getExits();
    }

    public int setItems() {
            Random rand = new Random();
            Scanner input = new Scanner(System.in);

            int itemSpawned = rand.nextInt(100) + 1;

            if (itemSpawned >= 1 && itemSpawned <= 5) {
                System.out.println("This room contains an Epic Sword! How lucky!");
                System.out.println("Would you like to take it?");
                System.out.println("(y/n): ");

                String choice = input.next();
                if (choice.equalsIgnoreCase("y")) {
                    return 3; //Epic sword
                }
                else {
                    return 0;
                }
            }
            else if (itemSpawned >=6 && itemSpawned <= 55) {
                System.out.println("This room contains a Health Potion!");
                System.out.println("Would you like to take it?");
                System.out.println("(y/n): ");

                String choice = input.next();
                if (choice.equalsIgnoreCase("y")) {
                    return 2; //Health potion
                }
                else {
                    return 0;
                }
            }
            else {
                System.out.println("This room contains a Mana Potion!");
                System.out.println("Would you like to take it?");
                System.out.println("(y/n): ");

                String choice = input.next();
                if (choice.equalsIgnoreCase("y")) {
                    return 1; //Mana potion
                }
                else {
                    return 0;
                }
            }
    }
}
