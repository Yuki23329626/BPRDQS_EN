from mnemonic import Mnemonic

mnemo = Mnemonic("english")

words = 'method banner ordinary buyer series either initial drink exchange rifle little lunch'

entropy = mnemo.to_entropy(words)

print(''.join('{:02x}'.format(x) for x in entropy))