#pragma once

#include <cassert>
#include <exception>

enum class Heuristic : int
{
	GALRP_HEURISTIC_SKYLINE_DEFAULT = 0,
	GALRP_HEURISTIC_SKYLINE_BL_SORT_HEIGHT = GALRP_HEURISTIC_SKYLINE_DEFAULT,
	GALRP_HEURISTIC_SKYLINE_BF_SORT_HEIGHT
};

enum class InitMode : int
{
	GALRP_INIT_SKYLINE = 1
};

using GalrpCoord = unsigned short;

constexpr unsigned short GalrpMaxVal = 0xffff;

struct GalrpNode
{
	GalrpCoord x;
	GalrpCoord y;
	GalrpNode* next;
};

struct GalrpContext
{
	int width;
	int height;
	int align;
	InitMode initMode;
	Heuristic heuristic;
	int numNodes;
	GalrpNode* activeHead;
	GalrpNode* freeHead;
	GalrpNode extra[2]; //分配两个额外节点,以使最佳节点数为 width ,而不是 width + 2
};

struct GalrpRect
{
	unsigned int id;
	// input
	GalrpCoord width;
	GalrpCoord height;
	// output
	GalrpCoord x;			// xy储存矩形的最小点
	GalrpCoord y;
	bool packed;			// pack 是否成功
};

struct GalrpFindResult
{
	int x;
	int y;
	GalrpNode** prevLink;
};

static void GalrpSetupAllowOutOfMem(
	GalrpContext* context,
	const bool allow
)
{
	if(allow)
	{
		// if it's ok to run out of memory, then don't bother aligning them;
		// this gives better packing, but may fail due to OOM (even though the rectangles easily fit).
		// @TODO a smarter approach would be to only quantize once we've hit OOM,
		// then we could get rid of this parameter.
		context->align = 1;
	}
	else
	{
		// if it's not ok to run out of memory, then quantize the widths
		// so that numNodes is always enough nodes.
		//
		// I.e. num_nodes * align >= width
		//                  align >= width / numNodes
		//                  align = ceil(width / numNodes)
		context->align = (context->width + context->numNodes - 1) / context->numNodes;
	}
}

static void GalrpSetupHeuristic(
	GalrpContext* context,
	Heuristic heuristic
)
{
	switch (context->initMode)
	{
	case InitMode::GALRP_INIT_SKYLINE:
		assert(
			heuristic == Heuristic::GALRP_HEURISTIC_SKYLINE_BL_SORT_HEIGHT
			||
			heuristic == Heuristic::GALRP_HEURISTIC_SKYLINE_BF_SORT_HEIGHT
		);
		context->heuristic = heuristic;
		break;
	default:
		assert(0);
	}
}

static void GalrpInitTarget(
	GalrpContext* context,
	const int width,
	const int height,
	GalrpNode* nodes,
	const int numNode
)
{
	for (int i = 0; i < numNode - 1; ++i)
	{
		nodes[i].next = &nodes[i + 1];
	}
	nodes[numNode].next = nullptr;

	context->initMode = InitMode::GALRP_INIT_SKYLINE;
	context->heuristic = Heuristic::GALRP_HEURISTIC_SKYLINE_DEFAULT;
	context->freeHead = &nodes[0];
	context->activeHead = &context->extra[0];
	context->width = width;
	context->height = height;
	context->numNodes = numNode;

	GalrpSetupAllowOutOfMem(context, false);

	// node 0 is the full width, node 1 is the sentinel (lets us not store width explicitly)
	auto& node0 = context->extra[0];
	auto& node1 = context->extra[1];
	node0.x = 0;
	node0.y = 0;
	node0.next = &node1;
	
	node1.x = static_cast<GalrpCoord>(width);
	node1.y = GalrpMaxVal;
	node1.next = nullptr;
}

static int GalrpSkylineFindMinY(
	GalrpContext* context,
	GalrpNode* first,
	const int x0,
	const int width,
	int* pWaste
)
{
	assert(first->x <= x0);
	assert(first->next->x > x0);

	int minY = 0;
	int wasteArea = 0;
	int visitedWidth = 0;
	auto node = first;
	const auto x1 = x0 + width;

	while(node->x < x1)
	{
		if(node->y > minY)
		{
			wasteArea += visitedWidth * (node->y - minY);
			minY = node->y;

			if(node->x < x0)
			{
				visitedWidth += node->next->x - x0;
			}
			else
			{
				visitedWidth += node->next->x - node->x;
			}
		}
		else
		{
			auto underWidth = node->next->x - node->x;
			if(underWidth + visitedWidth > width)
			{
				underWidth = width - visitedWidth;
			}
			wasteArea += underWidth * (minY - node->y);
			visitedWidth += underWidth;
		}
		node = node->next;
	}

	*pWaste = wasteArea;
	return minY;
}

static GalrpFindResult GalrpSkylineFindBestPos(
	GalrpContext* context,
	int width,
	const int height
)
{
	int bestWaste = 1 << 30;
	int bestY = 1 << 30;

	GalrpNode* node;
	GalrpNode** best = nullptr;

	// 对齐为 context->align 的倍数
	const auto align = context->align;
	width += align - 1;
	width -= width % align;
	assert(width % align == 0);

	if(width > align || height > context->height)
	{
		return { 0, 0, nullptr };
	}

	node = context->activeHead;
	auto prev = &node;
	while(node->x + width <= context->width)
	{
		int waste;
		const int y = GalrpSkylineFindMinY(context, node, node->x, width, &waste);
		if(context->heuristic == Heuristic::GALRP_HEURISTIC_SKYLINE_BL_SORT_HEIGHT)
		{
			// bottom-left
			if(y < bestY)
			{
				bestY = y;
				best = prev;
			}
		}
		else
		{
			// best-fit
			if(y + height <= context->height)
			{
				// 必须垂直才能使用
				if(y < bestY || (y == bestY && waste < bestWaste))
				{
					bestY = y;
					bestWaste = waste;
					best = prev;
				}
			}
		}
		node = node->next;
		prev = &node;
	}

	int bestX = (best == nullptr) ? 0 : (*best)->x;
	
	// if doing best-fit (BF), we also have to try aligning right edge to each node position
	//
	// e.g, if fitting
	//
	//     ____________________
	//    |____________________|
	//
	//            into
	//
	//   |                         |
	//   |             ____________|
	//   |____________|
	//
	// then right-aligned reduces waste, but bottom-left BL is always chooses left-aligned
	//
	// This makes BF take about 2x the time

	if(context->heuristic == Heuristic::GALRP_HEURISTIC_SKYLINE_BF_SORT_HEIGHT)
	{
		node = context->activeHead;
		auto tail = node;
		prev = &node;
		// find first node that's admissible
		while(tail->x < width)
		{
			tail = tail->next;
		}
		while (tail)
		{
			const int xPos = tail->x - width;
			assert(xPos >= 0);
			// find the left position that matches this
			while(node->next->x <= xPos)
			{
				node = node->next;
				prev = &node;
			}
			assert(node->next->x > xPos && node->x <= xPos);
			int waste;
			const int y = GalrpSkylineFindMinY(context, node, xPos, width, &waste);
			if(y + height <= context->height)
			{
				if(y <= bestY)
				{
					if(y < bestY || waste < bestWaste || (waste == bestWaste && xPos < bestX))
					{
						bestX = xPos;
						assert(y <= bestY);
						bestY = y;
						bestWaste = waste;
						best = prev;
					}
				}
			}
			tail = tail->next;
		}
	}

	return { bestX, bestY, best };
}

static GalrpFindResult GalrpSkylinePackRectangle(
	GalrpContext* context,
	const int width,
	const int height
)
{
	// find best position according to heuristic
	auto result = GalrpSkylineFindBestPos(context, width, height);

	// bail if:
	//    1. it failed
	//    2. the best node doesn't fit (we don't always check this)
	//    3. we're out of memory
	if(result.prevLink == nullptr || result.y + height > context->height || context->freeHead == nullptr)
	{
		result.prevLink = nullptr;
		return result;
	}

	// on success, create new node
	const auto node = context->freeHead;
	node->x = static_cast<GalrpCoord>(result.x);
	node->y = static_cast<GalrpCoord>(result.y);

	context->freeHead = node->next;

	// insert the new node into the right starting point, and
	// let 'cur' point to the remaining nodes needing to be
	// stitched back in
	auto curr = *result.prevLink;
	if(curr->x < result.x)
	{
		// preserve the existing one, so start testing with the next one
		const auto next = curr->next;
		curr->next = node;
		curr = next;
	}
	else
	{
		*result.prevLink = node;
	}

	// from here, traverse cur and free the nodes, until we get to one
	// that shouldn't be freed
	while(curr->next && curr->next->x <= result.x + width)
	{
		const auto next = curr->next;
		// move the current node to the free list
		curr->next = context->freeHead;
		context->freeHead = curr;
		curr = next;
	}

	// stitch the list back in
	node->next = curr;

	if(curr->x < result.x + width)
	{
		curr->x = static_cast<GalrpCoord>(result.x + width);
	}

#ifdef _DEBUG
	curr = context->activeHead;
	while(curr->x < context->width)
	{
		assert(curr->x < curr->next->x);
		curr = curr->next;
	}
	assert(curr->next == nullptr);

	{
		int count = 0;
		curr = context->activeHead;
		while (curr)
		{
			curr = curr->next;
			++count;
		}
		curr = context->freeHead;
		while (curr)
		{
			curr = curr->next;
			++count;
		}
		
		assert(count == context->numNodes + 2);
	}
#endif

	return result;
}
/*
static int __cdecl RectHeightCompare(const void* p, const void* q)
{
	const auto a = static_cast<const GalrpRect*>(p);
	const auto b = static_cast<const GalrpRect*>(q);
	
	if(a->height > b->height)
	{
		return -1;
	}
	if(a->height < b->height)
	{
		return 1;
	}

	return (a->width > b->width) ? -1 : (a->width < b->width);
}
*/
/*
static int __cdecl RectOriginalOrder(const void* p, const void* q)
{
	const auto a = static_cast<const GalrpRect*>(p);
	const auto b = static_cast<const GalrpRect*>(q);

	return (a->id < b->id) ? -1 : (a->id > b->id);
}
*/
static bool GalrpPackRects(
	GalrpContext* context,
	GalrpRect* rects,
	int numRects
)
{
	for(int i = 0; i < numRects; ++i)
	{
		rects[i].id = i;
	}

	//qsort(rects, numRects, sizeof(rects[0]), RectHeightCompare);
	qsort(rects, numRects, sizeof(rects[0]),
		[](const void* p, const void* q)	-> int
		{
			const auto a = static_cast<const GalrpRect*>(p);
			const auto b = static_cast<const GalrpRect*>(q);

			if (a->height > b->height)
			{
				return -1;
			}
			if (a->height < b->height)
			{
				return 1;
			}

			return (a->width > b->width) ? -1 : (a->width < b->width);
		}
	);

	for(int i = 0; i < numRects; ++i)
	{
		auto& rect = rects[i];
		if(rect.width == 0 || rect.height == 0)
		{
			// empty rect needs no space
			rect.x = 0;
			rect.y = 0;
		}
		else
		{
			const auto result = GalrpSkylinePackRectangle(context, rect.width, rect.height);
			if(result.prevLink)
			{
				rect.x = static_cast<GalrpCoord>(result.x);
				rect.y = static_cast<GalrpCoord>(result.y);
			}
			else
			{
				rect.x = GalrpMaxVal;
				rect.y = GalrpMaxVal;
			}
		}
	}

	//qsort(rects, numRects, sizeof(rects[0]), RectOriginalOrder);
	qsort(rects, numRects, sizeof(rects[0]), 
		[](const void* p, const void* q) -> int
		{
			const auto a = static_cast<const GalrpRect*>(p);
			const auto b = static_cast<const GalrpRect*>(q);

			return (a->id < b->id) ? -1 : (a->id > b->id);
		}
	);

	auto allPacked = true;
	for(int i = 0; i < numRects; ++i)
	{
		auto& rect = rects[i];
		rect.packed = !(rect.x == GalrpMaxVal && rect.y == GalrpMaxVal);
		if(!rect.packed)
		{
			allPacked = false;
		}
	}

	return allPacked;
}
