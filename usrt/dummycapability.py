class DummyCapability:
	def __init__(self):
		self.key = "f02e326f800ee26f04df7961adbf7c0a"

	def run(self,argv):
		print 'i am python'+str(argv)
		
	def getKey(self):
		return self.key
		
	def destroy(self):
		return
		
def newFun():
	return DummyCapability()
	