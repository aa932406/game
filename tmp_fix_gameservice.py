#!/usr/bin/env python3
"""Transform GameService.cpp from decompiler output to proper C++."""
import re

with open("src/Game/GameService.cpp", "r") as f:
    content = f.read()

# 1. Fix iterator variable declarations - remove raw _Rb_tree_iterator/_List_const_iterator
# and replace with auto

# 2. Fix ::operator->(&var)->second -> var->second
content = re.sub(
    r'std::_Rb_tree_iterator<std::pair<[^>]+>>::operator->\(&(\w+)\)->second',
    r'\1->second',
    content
)
content = re.sub(
    r'std::_List_const_iterator<[^>]+>::operator->\(&(\w+)\)',
    r'\1.operator->()',
    content
)

# 3. Fix ::operator!=(&a, &b) -> a != b  
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]+>::operator!=\(&(\w+),\s*&(\w+)\)',
    r'\1 != \2',
    content
)
content = re.sub(
    r'std::_List_const_iterator<[^>]+>::operator!=\(&(\w+),\s*&(\w+)\)',
    r'\1 != \2',
    content
)
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]+>::operator!=\(&(\w+),\s*&(\w+)\)',
    r'\1 != \2',
    content
)

# 4. Fix ::operator++(&var, 0) -> ++var
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]+>::operator\+\+\(&(\w+),\s*\d+\)',
    r'++\1',
    content
)
content = re.sub(
    r'std::_List_const_iterator<[^>]+>::operator\+\+\(&(\w+)\)',
    r'++\1',
    content
)
content = re.sub(
    r'std::_Rb_tree_const_iterator<[^>]+>::operator\+\+\(&(\w+)\)',
    r'++\1',
    content
)

# 5. Fix ::operator==(&a, &b) -> a == b
content = re.sub(
    r'std::_Rb_tree_iterator<[^>]+>::operator==\(&(\w+),\s*&(\w+)\)',
    r'\1 == \2',
    content
)

# 6. Fix std::map<...>::begin(&map) -> map.begin()
content = re.sub(
    r'std::map<(?:long|int|int64_t)\s*,\s*(?:Player|CObjPet|Monster|Trailer|int)\s*\*>\s*>::begin\(&this->(\w+)\)\._M_node',
    r'this->\1.begin()._M_node',
    content
)
content = re.sub(
    r'std::map<(?:int|int8_t)\s*,\s*(?:ConnType|int|ChatValidate)\s*\*?>\s*>::begin\(&this->(\w+)\)\._M_node',
    r'this->\1.begin()._M_node',
    content
)
content = re.sub(
    r'std::map<(?:int|int8_t)\s*,\s*(?:ConnType|int|ChatValidate)\s*\*?>\s*>::end\(&this->(\w+)\)\._M_node',
    r'this->\1.end()._M_node',
    content
)

# 7. Fix std::map<...>::end(&map) -> map.end()
content = re.sub(
    r'std::map<(?:long|int|int64_t)\s*,\s*(?:Player|CObjPet|Monster|Trailer|int)\s*\*>\s*>::end\(&this->(\w+)\)\._M_node',
    r'this->\1.end()._M_node',
    content
)

# 8. Fix std::map<...>::find(&map, &key) -> map.find(key)
content = re.sub(
    r'std::map<(?:long|int|int64_t)\s*,\s*(?:Player|CObjPet|Monster|Trailer|int|ChatValidate)\s*\*?\s*>::find\(&this->(\w+),\s*&(\w+)\)\._M_node',
    r'this->\1.find(\2)._M_node',
    content
)

# 9. Fix std::map<...>::operator[](&map, &key) -> map[key]
content = re.sub(
    r'\*std::map<(?:long|int|int64_t)\s*,\s*(?:Player|CObjPet|Monster|Trailer|int|long)\s*\*?\s*>::operator\[\]\(&this->(\w+),\s*&(\w+)\)',
    r'this->\1[\2]',
    content
)

# 10. Fix std::map<...>::empty(&map) -> map.empty()
content = re.sub(
    r'std::map<(?:long|int|int64_t)\s*,\s*(?:Player|CObjPet|Monster|Trailer|int)\s*\*?\s*>::empty\(&this->(\w+)\)',
    r'this->\1.empty()',
    content
)

# 11. Fix std::map<...>::clear(&map) -> map.clear()
content = re.sub(
    r'std::map<(?:int|int8_t)\s*,\s*int\s*>::clear\(&this->(\w+)\)',
    r'this->\1.clear()',
    content
)

# 12. Fix std::list<...>::begin(&list) -> list.begin()
content = re.sub(
    r'std::list<(?:long|short|Player\s*\*|int)>\s*>::begin\(&(\w+)\)\._M_node',
    r'\1.begin()._M_node',
    content
)
content = re.sub(
    r'std::list<(?:long|short|Player\s*\*|int)>\s*>::end\(&(\w+)\)\._M_node',
    r'\1.end()._M_node',
    content
)

# 13. Fix std::list<...>::push_back(&list, &val) -> list.push_back(val)
content = re.sub(
    r'std::list<(?:short|long|Player\s*\*)>\s*>::push_back\(&(\w+),\s*&(\w+)\)',
    r'\1.push_back(\2)',
    content
)

# 14. Fix std::list<...>::size(&list) -> list.size()
content = re.sub(
    r'std::list<(?:short|long)>\s*>::size\(&(\w+)\)',
    r'\1.size()',
    content
)

# 15. Fix std::list<...>::empty(&list) -> list.empty()
content = re.sub(
    r'std::list<(?:long|short)>\s*>::empty\(&(\w+)\)',
    r'\1.empty()',
    content
)

# 16. Fix std::vector<...>::vector(&vec) -> remove (vec is constructed)
content = re.sub(
    r'std::vector<MemChrBag>::vector\(&(\w+)\);\s*',
    r'std::vector<MemChrBag> \1;\n',
    content
)

# 17. Fix std::vector<...>::~vector(&vec) -> remove
content = re.sub(
    r'std::vector<MemChrBag>::~vector\(&(\w+)\);',
    r'/* \1 destructed automatically */',
    content
)

# 18. Fix std::vector<...>::push_back(&vec, &val) -> vec.push_back(val)
content = re.sub(
    r'std::vector<MemChrBag>::push_back\(&(\w+),\s*&(\w+)\)',
    r'\1.push_back(\2)',
    content
)

# 19. Fix std::vector<...>::operator[](&vec, i) -> vec[i]
content = re.sub(
    r'std::vector<MemChrBag>::operator\[\]\(&(\w+),\s*(\w+)\)',
    r'\1[\2]',
    content
)

# 20. Fix std::vector<...>::resize(&vec, n, val) -> vec.resize(n, val)
content = re.sub(
    r'std::vector<MemChrBag>::resize\(&(\w+),\s*(\w+),\s*(\w+)\)',
    r'\1.resize(\2, \3)',
    content
)

# 21. Fix ::_M_node access for iterator declarations  
# Replace "iter._M_node = something._M_node;" with "iter = something;"
content = re.sub(
    r'(\w+)\._M_node\s*=\s*(.+?)\._M_node;',
    r'\1 = \2;',
    content
)

# 22. Remove raw vtable variable declarations
content = re.sub(
    r'\s+(int32_t|int64_t|char|bool)\s+\w+;\s+// (rbx|rax|bl|dl|cl|rcx|rdx|r8|r9|r10|r11|r12|r13|r14|r15)\s*',
    r'  /* var */',
    content
)

# 23. Remove decompiler register comments like // rax, // rbx etc
content = re.sub(
    r'// (rax|rbx|rcx|rdx|rsi|rdi|r8|r9|r10|r11|r12|r13|r14|r15|rsp|rbp|bl|cl|dl|al|sil|dil)',
    r'',
    content
)

# 24. Fix static method calls with explicit this
# GameService::method(this, ...) -> method(...)
content = re.sub(
    r'GameService::(\w+)\(this,\s*',
    r'\1(',
    content
)

# 25. Fix MutiConn<GameService>::GetId -> just GetId (since it's ConnType)
content = re.sub(
    r'MutiConn<GameService>::GetId\((\w+)\)',
    r'\1->GetId()',
    content
)

# 26. Fix Answer::TcpClient static calls - these are pConn->xxx
content = re.sub(
    r'Answer::TcpClient::connect\((\w+),\s*&(\w+)\)',
    r'\1->connect(\2)',
    content
)
content = re.sub(
    r'Answer::TcpClient::start\((\w+)\)',
    r'\1->start()',
    content
)
content = re.sub(
    r'Answer::TcpClient::sendPacket\((\w+),\s*(\w+)\)',
    r'\1->sendPacket(\2)',
    content
)
content = re.sub(
    r'Answer::TcpClient::popNetpacket\((\w+)\)',
    r'\1->popNetpacket()',
    content
)
content = re.sub(
    r'Answer::TcpClient::popNetpacket\((\w+),\s*(\w+)\)',
    r'\1->popNetpacket(\2)',
    content
)
content = re.sub(
    r'Answer::TcpClient::getName\((\w+)\)',
    r'\1->getName()',
    content
)

# 27. Fix Answer::TcpService static calls
content = re.sub(
    r'Answer::TcpService::sendPacketTo\((\w+),\s*(\w+),\s*(\w+)\)',
    r'\1->sendPacketTo(\2, \3)',
    content
)
content = re.sub(
    r'Answer::TcpService::replySuccess\((\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)',
    r'\1->replySuccess(\2, \3, \4)',
    content
)
content = re.sub(
    r'Answer::TcpService::replyfailure\((\w+),\s*(\w+),\s*(\w+),\s*(\w+),\s*(\w+)\)',
    r'\1->replyfailure(\2, \3, \4, \5)',
    content
)

# 28. Fix map[indexMap construction
content = re.sub(
    r'std::map<int8_t,std::list<short>>::map\(&(\w+)\)',
    r'IndexMap \1',
    content
)
content = re.sub(
    r'std::map<int8_t,std::list<short>>::~map\(&(\w+)\)',
    r'/* \1 destructed */',
    content
)

# 29. Fix list construction  
content = re.sub(
    r'std::list<long>::list\(&(\w+)\)',
    r'CharIdList \1',
    content
)
content = re.sub(
    r'std::list<long>::~list\(&(\w+)\)',
    r'/* \1 destructed */',
    content
)

# 30. Remove manual string construction/destruction (they auto-manage)
content = re.sub(
    r'std::string::string\(&(\w+)\);',
    r'/* string \1 auto-constructed */',
    content
)
content = re.sub(
    r'std::string::~string\(&(\w+)\);',
    r'/* \1 auto-destructed */',
    content
)

with open("src/Game/GameService.cpp", "w") as f:
    f.write(content)

print("Transformations applied successfully.")
