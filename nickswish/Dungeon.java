public class Dungeon {

    //declarations for each room
    private Room cockpit;
    private Room leftHall;
    private Room captainsBedroom;
    private Room rightHall;
    private Room coCapBedroom;
    private Room shipBack;
    private Room storage;
    private Room escapePods;


    //constructor dungeon
    public Dungeon() {
        cockpit = new Room("Cockpit of the Ship");
        leftHall = new Room("Left Hall");
        captainsBedroom = new Room("Captain's Bedroom");
        rightHall = new Room("Right Hall");
        coCapBedroom = new Room("Co-Captain's Bedroom");
        shipBack = new Room("Back of the Ship");
        storage = new Room("Storage Closet");
        escapePods = new Room("Escape Pods");

        this.cockpit.setExits(shipBack, rightHall, leftHall, null);
        this.leftHall.setExits(null, cockpit, null, captainsBedroom);
        this.captainsBedroom.setExits(leftHall, null, null, null);
        this.rightHall.setExits(null,null, cockpit, coCapBedroom);
        this.coCapBedroom.setExits(rightHall, null, null, null);
        this.shipBack.setExits(null, escapePods, storage, cockpit);
        this.storage.setExits(null, shipBack, null, null);
        this.escapePods.setExits(null, null, shipBack, null);
    }

    //returns the starting room of the dungeon which is the ship's cockpit
    public Room getStartingRoom() {
        return this.cockpit;
    }

}
