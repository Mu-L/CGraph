"""
@Author: Chunel
@Contact: chunel@foxmail.com
@File: MyWriteParamNode
@Time: 2025/2/27 23:42
@Desc: 
"""

from pycgraph import GNode, CStatus

from MyParams.MyParam import MyParam

class MyWriteParamNode(GNode):
    def init(self):
        return self.createGParam(MyParam(), "param1")

    def run(self):
        param: MyParam = self.getGParam("param1")
        with param:
            # user `with param` to enter thread safe area for param's info change
            # as same as param.lock() and param.unlock()
            param.count += 1
            param.value += 1
            print('[{0}] value is {1}, count is {2}'.format(self.getName(), param.value, param.count))
        return CStatus()
