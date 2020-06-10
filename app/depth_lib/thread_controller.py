import threading


class Thread(threading.Thread):
    counter = 0
    condition = None
    groups = {}
    all_threads = []

    def __init__(self, fn, args, thread_condition=True):
        threading.Thread.__init__(self)
        self.fn = fn
        self.args = args
        self.thread_condition = thread_condition
        Thread.static_init()

    def run(self):
        if self.thread_condition:
            self.fn(self.args, Thread.condition)
        else:
            self.fn(self.args)

    @staticmethod
    def static_init():
        Thread.condition = Thread.condition or threading.Condition()

    @staticmethod
    def register_group(group_name):
        Thread.groups[group_name] = threading.Condition()

    @staticmethod
    def join_all():
        for thread in Thread.all_threads:
            thread.join()
