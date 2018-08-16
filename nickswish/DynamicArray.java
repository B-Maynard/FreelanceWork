import java.util.Arrays;

public class DynamicArray {

    private String[] array;
    private int sizeOfArray;
    private int nextIndex;


    //Default constructor that will allocate an array of size 10
    public DynamicArray() {
        this.array = new String[10];
        this.sizeOfArray = 0;
        this.nextIndex = 0;
    }

    //Constructor that allows the user to specify a size
    public DynamicArray(int size) {
        this.array = new String[size];
        this.sizeOfArray = 0;
        this.nextIndex = 0;
    }

    //Constructor that will create an array based on an already created array
    public DynamicArray(String[] givenArray) {
        this.array = Arrays.copyOf(givenArray, givenArray.length);
        this.sizeOfArray = givenArray.length;
        this.nextIndex = givenArray.length;
    }

    //Function to add a new value to the array. Will add the value to the next available space
    // If the next space is greater than that of the size of the array, the array will expand itself 1.5 times
    public void add(String newValue) {
        if (this.array.length == this.nextIndex) {
            this.expandArray();
        }
        this.array[nextIndex] = newValue;
        this.sizeOfArray++;
        this.nextIndex++;
    }

    //Will remove the value at the specified index and return the value that was deleted
    public String remove(int index) {
        String removedString = this.array[index];

        if (index < 0 || index > this.sizeOfArray) {
            return null;
        }

        for (int i = index; i < this.sizeOfArray - 1; i++) {
            this.array[i] = this.array[i + 1];
        }

        this.nextIndex--;
        this.sizeOfArray--;
        return removedString;
    }

    //Will return the value at the index specified by the user
    public String get(int index) {
        return this.array[index];
    }

    //Will return a true or false value based on if the size of the array is 0 or not
    public boolean isEmpty() {
        return (this.sizeOfArray == 0);
    }

    //Will return the current size of the array
    public int sizeOf() {
        return this.sizeOfArray;
    }


    //Private method that will expand the array to 1.5 times the current size if the array runs out of room
    private void expandArray() {
        int newSize = this.array.length * 3 / 2;
        String [] newArray = new String[newSize];

        for (int i = 0; i < this.array.length; i++) {
            newArray[i] = this.array[i];
        }

        this.array = newArray;
    }
}
