```
unit->runAnimation(anim) {
	body->runAnimation(anim) {
		// body 파츠마다 runAction
		for part in parts {
			part_animation = anim.getPartAnimation(id)
			// part_animation이 없으면 무시
			continue if !part_animation
			// 이미 같은 걸 재생중이라면 무시
			continue if part_animation == playing_animation[id]
			// 다른 애니메이션이 재생중이라면 stopAnimation 하고 재생
			stopAnimtion(id) if part_animation != playing_animation[id]
			// Animation은 1회용 또는 무한반복
			anim->runAnimationPart(part, id) {
				part->runAction(Animate(part_animation[id]))
			}
		}
	}
}

body->stopAnimation(id) {
	// 애니메이션 중지
	parts[id]->stopAllActions()
	// 원래 Sprite로 복구
	parts[id] = origin_parts[id]
}

unit->stopAllAnimation() {
	body->stopAllAnimation() {
		stopAnimation(1..id)
	}
}
```

```
class Unit < Sprite
	function runAnimation(bodyAnim)
	function stopAllAnimation()

	Body body
end
```

```
class Body < Sprite
	function runAnimation(bodyAnim)
	function stopAnimation(id)
	function stopAllAnimation()

	Sprite[] parts
	Sprite[] original_parts
	Animation[] playing_animation;
end
```

```
class BodyAnimation < Ref
	function create(filename, parts, frame)
	function init(filename, parts, frame)
		for i in parts
			for j in 1..frame
				part_animation[i]->addSpriteFrame("{filename}_{j}_{i}.png");
			end
		end
	end
	function getPartAnimation(id)

	Animation[] part_animation
end
```

```
class AnimationPool < Singleton
	function getAnimation(name)
	function loadFromFile(filename)

	map<string, BodyAnimation> pool
end
```


#### animation.json
``` JSON
{
	"character1" : {
		"parts" : [1, 2, 3, 4, 5, 6],
		"frame" : 8,
	},

	"character2" : {
		"parts" : [5, 6],
		"frame" : 4,
	},
}
```
