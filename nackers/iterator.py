class Pair:
    def __init__(self, argList1, argList2):
        self.list1 = argList1
        self.list2 = argList2
        self.i = 0

    def __iter__(self):
        return self

    def __next__(self):
        print (len(self.list1))
        print (len(self.list2))
        if self.i > len(self.list1) and self.i > len(self.list2): #if i ever exceeds both lists, stop iteration
            raise StopIteration
        elif len(self.list1) == len(self.list2): #if the lists are the same size, return pairs without exception
            result = (self.list1[self.i], self.list2[self.i])
            self.i = self.i + 1
            return result
        elif len(self.list1) < len(self.list2): #if list1 is smaller, print None when you run out of items
            if self.i >= len(self.list1):
                result = (None, self.list2[self.i])
                self.i = self.i + 1
                return result
            else:
                result = (self.list1[self.i], self.list2[self.i]) #if you're not out of items yet, print normal
                self.i = self.i + 1
                return result
        else:
            if self.i >= len(self.list2): #Only other option is that list2 is less than list 1
                                    #if i goes past list2, then print None when out of items
                result = (self.list1[self.i], None)
                self.i = self.i + 1
                return result
            else:
                result = (self.list1[self.i], self.list2[self.i]) #otherwise, print normal
                self.i = self.i + 1
                return result




def quick_test():
	"""
	This allows you to quickly test the elements of your iterator
	"""
	l = [1, 2, 3, 4, 5]
	k = [0, 9, 8, 7, 6]
	pair = iter(Pair(l, k))
	print(next(pair) == (1, 0))
	print(next(pair) == (2, 9))
	print(next(pair) == (3, 8))


quick_test()
