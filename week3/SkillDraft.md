```
class Skill interface
  static string tooltip;
  static string name;

  static int id;

  int level = 0; // 안배우면 0, 배우면 1~
end
```
```
class ActiveSkill < Skill
  static float duration; // 방향, 이동 잠금 시간
  static float cooltime; // 다음 스킬까지 쿨타임
  static float cost; // MP cost
  
  function use(
    Player *p,
    Vec2 target ); // target은 마우스 클릭 위치
end
```
```
class PassiveSkill < Skill
  map<string targetAttribute, Attribute> bonusAttributes;
end
```

```
class Attribute
  float value;
  float bonusRate;
  float bonusValue;
 
  function get(); // value에 bonus 더한 총량 계산해서 반환

  function setBonusRate();
  function getBonusRate();

  function setBonusValue();
  function getBonusValue();
end
```
```
class Player
  vec<Skill *> skills;
  float cooltime;
  float duration;

  function canUse(
    id SkillID ){

    // 쓸 수 있는 스킬인지 검사
    // 쿨타임검사
    // 잔여 mp검사
  }

  function useSkill(
    id skillID ){
    
    // 스킬 발동
    // 쿨타임, 경직 갱신
    // mp 감소
  }
end
```
<br><br>
#### passive_skill.json
```JSON
{
  "id" : 10,
  "name" : "power",
  "tooltip" : "쎄짐",

  "hp" : {
    "value" : 10, // + 증가치
    "rate" : 1 // % 증가치
  },
  "attack" : {
    "value" : 0,
    "rate" : 10
  }
}
```
#### player_type.json
```JSON
{
  "id" : 0,
  "skills" : [
    10, 15, 20, 25
  ]
}
```
