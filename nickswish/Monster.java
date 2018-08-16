import java.util.Random;
import java.util.Scanner;

//Monster is a child of GameCharacter
public class Monster extends GameCharacter {

    private int xP;

    //Monster's constructor
    public Monster(String name, int health, int attackPower, int xP) {
        this.setName(name);
        this.setHealth(health);
        this.setAttackPower(attackPower);
        this.xP = xP;
    }

    //Will allow the monster to inflict damage on the player
    public void attack(Player player) {
        player.takeDamage(this.getAttackPower());
    }

    //Allows the monster to take its turn. It can only use an attack move
    public void takeTurn(Player player) {
        this.attack(player);
        System.out.println("The " + this.getName() + " attacked you!");
        try {
            Thread.sleep(1000);
        }
        catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }

    //Will return the xp gained from killing this monster.
    public int getXP() {
        return this.xP;
    }

    //Will return the values of the attributes of the monster
    public String toString() {
        String output = "\nName: " + this.getName() + "\nHealth: " + this.getHealth() + "\nAttack Power: " + this.getAttackPower() + "XP Worth: " + this.getXP();

        return output;
    }

    //Sets the random item that the monster will drop when defeated
    public int setItems() {
        Random rand = new Random();
        Scanner input = new Scanner(System.in);

        //Will generate a random number from 1-100
        int itemSpawned = rand.nextInt(100) + 1;

        //The Epic Sword has a 5% chance to drop, since it adds a big buff to the player
        if (itemSpawned >= 1 && itemSpawned <= 5) {
            System.out.println("This monster was carrying an Epic Sword! How lucky!");
            System.out.println("Would you like to take it?");
            System.out.println("(y/n): ");

            //Checks if the player would like to put the item in their inventory
            String choice = input.next();
            if (choice.equalsIgnoreCase("y")) {
                return 3; //Epic sword
            }
            else {
                return 0;
            }
        }
        //the health potion has a 50% chance to drop
        else if (itemSpawned >=6 && itemSpawned <= 55) {
            System.out.println("This monster was carrying a Health Potion!");
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
            //If all else fails, drop a mana potion
            System.out.println("This monster was carrying a Mana Potion!");
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
