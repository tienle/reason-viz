type cb('a) = 'a => unit;

type t('a) = ref(list(cb('a)));

let create = () => ref([]);

let subscribe = (e: t('a), f: cb('a)) => {
  e := List.append(e^, [f]);
  let unsubscribe = () => {
    e := List.filter(f => f !== f, e^);
  };

  unsubscribe;
};

let dispatch = (e: t('a), v: 'a) => {
  List.iter(c => c(v), e^);
};

