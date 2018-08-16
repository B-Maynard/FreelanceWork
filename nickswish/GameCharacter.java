public class GameCharacter {

    private String name;
    private int health;
    private int attackPower;

    //Default constructor
    public GameCharacter() {
        this.name = "";
        this.health = 0;
        this.attackPower = 0;
    }

    //Constructor with parameters
    public GameCharacter(String name, int health, int attackPower) {
        this.name = name;
        this.health = health;
        this.attackPower = attackPower;
    }

    //Function to allow the game character to take the damage dealt to it
    public void takeDamage(int damage) {
        this.health -= damage;
    }


    /*The set of getter functions below will return the attributes of the game character*/
    public String getName() {
        return this.name;
    }
    public int getAttackPower() {
        return this.attackPower;
    }
    public int getHealth() {
        return this.health;
    }


    //Will output the values of each attribute of the game character
    public String toString() {
        String output;

        output = "\nName: " + this.name + "\nHealth: " + this.health + "\nAttack Power: " + this.attackPower;
        return output;
    }

    /*The setter functions will allow for the attributes of the game character to be set to specific values*/
    public void setName(String name) {
        this.name = name;
    }
    public void setHealth(int health) {
        this.health = health;
    }
    public void setAttackPower(int attackPower) {
        this.attackPower = attackPower;
    }
}
