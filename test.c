typedef struct set_t set_t;
struct set_t
{
	node_t *head;
};

set_t *new_set(node_t *node)
{
	set_t *set = calloc(1, sizeof(set_t));

	set->head = node;
	node->next = NULL;

	return set;
}

void addFirst(set_t *h, node_t *node)
{
	node_t *temp = h->head;
	h->head = node;
	node->next = temp;
}
