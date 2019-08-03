class BinTree:
	class Empty:
		def add(self,val):
			return BinTree.Leaf(val)
		def get(self):
			return []
		def find(self,val):
			return False
		def delete(self,val):
			return self
	class Leaf:
		def __init__(self,val):
			self.val = val
		def add(self,val):
			return BinTree.Node(self.val).add(val)
		def get(self):
			return [self.val]
		def find(self,val):
			return self.val == val 
		def delete(self,val):
			return BinTree.Empty()
	class Node:
		def __init__(self,val):
			self.val = val
			self.left = BinTree.Empty()
			self.right = BinTree.Empty()
		def add(self,val):
			if val <= self.val:
				self.left = self.left.add(val)
			else:
				self.right = self.right.add(val)
			return self
		def get(self):
			return self.left.get() + [self.val] + self.right.get()
		def find(self,val):
			if self.val == val:
				return True
			elif val < self.val:
				return self.left.find(val)
			else:
				return self.right.find(val)
		def delete(self,val):
			if self.val == val:
				if self.left.get():
					node_val = self.left.get()[-1]
					self.left = self.left.delete(node_val)
				elif self.right.get():
					node_val = self.right.get()[0]
					self.right = self.right.delete(node_val)
				node = BinTree.Node(node_val)
				node.left = self.left
				node.right = self.right
				return node
			elif val < self.val:
				self.left = self.left.delete(val)
			else:
				self.right = self.right.delete(val)
			return self
	def __init__(self):
		self.val = self.Empty()
	def add(self,val):
		self.val = self.val.add(val)
	def get(self):
		return self.val.get()
	def find(self,val):
		return self.val.find(val)
	def delete(self,val):
		self.val = self.val.delete(val)

bt = BinTree()
bt.add(5)
bt.add(3)
bt.add(7)
bt.add(10)
bt.add(6)
bt.add(4)
print(bt.get())
print(bt.find(2))
bt.delete(7)
bt.delete(6)
print(bt.get())
