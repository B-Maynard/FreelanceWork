import java.util.Scanner;

public class Player extends GameCharacter {

    private int mana;
    private int[] playerInventory = new int [10];
    private int equipedSwords = 0;

    //Player constructor
    public Player(String name, int health, int attackPower, int mana) {
        this.setName(name);
        this.setHealth(health);
        this.setAttackPower(attackPower);
        this.mana = mana;
    }

    //Allows the player to inflict damage on the monster
    public void attack(Monster monster) {
        monster.takeDamage(this.getAttackPower());
        System.out.println("You have attacked the " + monster.getName() + " with your sword!");
    }

    //Allows the user to cast a spell if they have enough mana to do so
    public void castSpell(Monster monster) {
        if (mana < 3) {
            System.out.println("You need to charge your mana to 3 in order to use that!");
            return;
        }

        monster.takeDamage(this.getAttackPower() * 4);
        System.out.println("You have casted a spell on the " + monster.getName() + "!");
        this.mana = 0;
    }

    //Allows the player to charge their mana for spell casting
    public void chargeMana() {
        this.mana++;
        System.out.println("You charge your mana for 1 point!");
    }

    //Allows the player to choose what attack they'd like to perform
    public void takeTurn(String choice, Monster monster) {
        if (choice.equalsIgnoreCase("1")) {
            this.attack(monster);
        }
        else if (choice.equalsIgnoreCase("2")) {
            this.castSpell(monster);
        }
        else if (choice.equalsIgnoreCase("3")) {
            this.chargeMana();
        }
        else if (choice.equalsIgnoreCase("4")) {
            System.out.println("You chose to run back to the previous room.");
        }
        //Lets the player access and use items from their inventory
        else if (choice.equalsIgnoreCase("5")) {
            this.accessInventory();
            this.equipItems();
        }
        else {
            System.out.println("That's not a valid command.");
        }
        try {
            Thread.sleep(1000);
        }
        catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }

    //Returns the value of the mana attribute
    public int getMana() {
        return this.mana;
    }

    //Returns the values of the attributes of the player
    public String toString() {
        String output = "\nName: " + this.getName() + "\nHealth: " + this.getHealth() + "\nAttack Power: " + this.getAttackPower() + "\nMana: " + this.getMana();

        return output;
    }

    //Will add items to the players inventory
    public void addToInventory(int itemId) {
        int i;
        //If there are less than 10 items in the inventory, add it
        for (i = 0; i < 10; i++) {
            if (this.playerInventory[i] == 0) {
                this.playerInventory[i] = itemId;
                break;
            }
        }

        //If there are 10 items in the player's inventory, they are out of space
        if (i == 10) {
            System.out.println("You are out of space in your inventory.");
        }
    }

    //Remove the item that the player used
    public void removeFromInventory(int place) {
        this.playerInventory[place] = 0;
    }

    //Print the contents of the player's inventory array
    public void accessInventory () {
        System.out.println("Inventory:");
        for (int i = 0; i < 10; i++) {
            System.out.print(i + 1 + ". ");
            if (this.playerInventory[i] == 1) {
                System.out.println("Mana Potion");
            }
            else if (this.playerInventory[i] == 2) {
                System.out.println("Health Potion");
            }
            else if (this.playerInventory[i] == 3) {
                System.out.println("Epic sword");
            }
            else {
                System.out.println("(Empty)");
            }
        }
    }

    //Allows the player to use items in their inventory
    public void equipItems() {
        int choice;
        Scanner input = new Scanner(System.in);

        System.out.println("Which of these items would you like to use?");
        System.out.print("(Enter Command. Press 0 to leave inventory.): ");

        choice = input.nextInt();
        if (choice == 0) {
            return;
        }

        //needs to be choice - 1, because the player isn't displayed a list of items starting at 0
        int itemId = this.playerInventory[choice - 1];

        if (itemId == 1) {
            //If the player tries to use a mana potion but is at full mana, don't let them use the potion
            if (this.getMana() >= 3) {
                System.out.println("You're already at full mana!");
                return;
            }
            System.out.println("You have used a Mana Potion!");
            this.mana += 3;
            this.removeFromInventory(choice - 1);
        }
        else if (itemId == 2) {
            //Same as the mana potion above. If the player's at full health, don't let them use the potion
            if (this.getHealth() >= 60) {
                System.out.println("You're already at full health!");
                return;
            }
            System.out.println("You have used a Health Potion!");
            this.setHealth(this.getHealth() + 10);
            this.removeFromInventory(choice - 1);
        }
        else if (itemId == 3) {
            //The player cannot equip more than 2 swords, because that would be overkill
            if (this.equipedSwords >= 2) {
                System.out.println("You cannot equip more than two swords!");
                return;
            }
            System.out.println("You have equipped an Epic Sword! (You may only equip 2 at a time!)");
            this.setAttackPower(this.getAttackPower() + 5);
            this.removeFromInventory(choice - 1);
        }
        else {
            System.out.println("There is no item in that slot.");
            return;
        }
    }
}
