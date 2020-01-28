import redis
import time
import unittest

ONE_WEEK_IN_SECONDS = 7*86400
VOTE_SCORE = 432
ARTICLES_PER_PAGE = 25
HOST = "121.36.51.156"
PORT = 6379


def article_vote(conn, user, article):
    cutoff = time.time()-ONE_WEEK_IN_SECONDS
    begintime = conn.zscore('time:', article)
    if begintime < cutoff:
        return
    article_id = article.partition(':')[-1]
    if conn.sadd('voted:' + article_id, user):
        conn.zincrby('score:', VOTE_SCORE, article)
        conn.hincrby(article, 'votes', 1)


def post_article(conn, user, title, link):
    article_id = conn.incr('article:')
    article_id = str(article_id)

    voted = 'voted:' + article_id
    conn.sadd(voted, user)
    conn.expire(voted, ONE_WEEK_IN_SECONDS)

    now = time.time()
    article = "article:" + article_id
    conn.hmset(article, {
        'title' : title,
        'link' : link,
        'poster' : user,
        'time' : now,
        'votes' : 1
    })
    conn.zadd('score:', {article: now+VOTE_SCORE})
    conn.zadd('time:', {article: now})

    return article_id


def get_articles(conn, page, order='score:'):
    start = (page-1)*ARTICLES_PER_PAGE
    end = start+ARTICLES_PER_PAGE-1
    ids = conn.zrevrange(order, start, end)
    articles = []
    for id in ids:
        article = conn.hgetall(id)
        article['id'] = id
        articles.append(article)
    return articles


def add_remove_groups(conn, article_id, to_add=[], to_remove=[]):
    article = 'article:' + article_id
    for g in to_add:
        conn.sadd('group:'+g, article)
    for g in to_remove:
        conn.srem('group:' + g, article)


def get_group_articles(conn, group, page, order='score:'):
    key = order+group
    if not conn.exists(key):
        conn.zinterstore(key,
            ['group:'+group, order],
            aggregate='max')
        conn.expire(key, 60)
    return get_articles(conn, page, key)


class TestCh01(unittest.TestCase):
    def setUp(self):
        self.conn = redis.Redis(host=HOST, port=PORT, db=15)

    def tearDown(self):
        del self.conn
        print()
        print()

    def test_article_functionality(self):
        conn = self.conn
        import pprint

        article_id = str(post_article(conn, 'username', 'A title', 'http://www.google.com'))
        print("We posted a new article with id:", article_id)
        print()
        self.assertTrue(article_id)

        print("Its HASH looks like:")
        r = conn.hgetall('article:' + article_id)
        print(r)
        print()
        self.assertTrue(r)

        article_vote(conn, 'other_user', 'article:' + article_id)
        print("We voted for the article, it now has votes:")
        v = int(conn.hget('article:' + article_id, 'votes'))
        print(v)
        print()
        self.assertTrue(v > 1)

        print("The currently highest-scoring articles are:")
        articles = get_articles(conn, 1)
        pprint.pprint(articles)
        print()

        self.assertTrue(len(articles) >= 1)

        add_remove_groups(conn, article_id, ['new-group'])
        print("We added the article to a new group, other articles include:")
        articles = get_group_articles(conn, 'new-group', 1)
        pprint.pprint(articles)
        print()
        self.assertTrue(len(articles) >= 1)

        to_del = (
            conn.keys('time:*') + conn.keys('voted:*') + conn.keys('score:*') +
            conn.keys('article:*') + conn.keys('group:*')
        )
        if to_del:
            conn.delete(*to_del)


if __name__ == '__main__':
    unittest.main()
